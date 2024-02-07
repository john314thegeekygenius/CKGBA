/*
	K1n9_Duk3's IMF to WAV converter - Converts IMF files to WAV.
	Copyright (C) 2013-2020 K1n9_Duk3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>

#include "fmopl.h"
#include "imf2wav.h"


//just a quick hack, but it works...
#pragma pack(2)		//this gave me a headache...
struct waveheader{
	UINT32 rID, rSize;
	UINT32 wID;
	UINT32 fID, fSize;
	UINT16 fFormat, fChannels;
	UINT32 fHertz, fBytesPerSec;
	UINT16 fBlockAlign, fBits, fSpecific;
	UINT32 dID, dSize;
};
#pragma pack()

waveheader head = 
{
	0x46464952,		//rID = "RIFF"
	0,				//rSize (dummy value)
	0x45564157,		//wID = "WAVE"
	0x20746D66,		//fID = "fmt "
	18,				//fSize
	1,				//fFormat
	1,				//fChannels
	DEFAULT_FREQ,		//fHertz
	0,				//fBytesPerSec (dummy value)
	4,				//fBlockAlign
	OPL_SAMPLE_BITS,//fBits
	0,				//fSpecific
	0x61746164,		//dID = "data"
	0				//dSize (dummy value)
};

UINT32 ReadInt32LE(UINT8 **in)
{
	UINT8 *data = *in;
	UINT32 result = 0;
	result = *data++; result <<= 8;
	result += (*data++); result <<= 8;
	result += (*data++); result <<= 8;
	result += (*data); *in += 4;
//	std::cout << "Read Data 32: " << result << std::endl;
	return result;
}

UINT16 ReadInt16LE(UINT8 **in)
{
	UINT8 *data = *in;
	UINT16 result = 0;
	result = *data++; result <<= 8;
	result += (*data); *in += 2;
//	std::cout << "Read Data 16: " << result << std::endl;
	return result;
}

UINT8 ReadByte(UINT8 **in)
{
	UINT8 *data = *in;
	UINT8 result = 0;
	result = *data; *in ++;
//	std::cout << "Read Data 8: " << result << std::endl;
	return result;
}

void ReadData(void *vdata, size_t size, int blocks, UINT8 **in)
{
	UINT8 *idata = *in;
	if(!vdata) {
		std::cout << "ERROR! Bad data ptr!" << std::endl;
		exit(1);
	}
	memcpy(vdata, idata, size*blocks);
//	std::cout << "Read Data V: " << *((uint32_t*)vdata) << std::endl;
	*in += (size*blocks);
}


int ConvertIMFToBin(std::vector<uint16_t> &outwav, 
					uint8_t *imfdata, 
					long datalen, 
					unsigned int imf_rate,
					unsigned int wav_rate)
{
	INT16 *buffer;
	unsigned int samples_per_tick = 0, isKMF = 0, cmds=0, ticks=0;
	UINT16 channel_mask=0xffff;

	//std::cout << "K1n9_Duk3's IMF to WAV converter v1.2" << std::endl;

	samples_per_tick=wav_rate/imf_rate;
	if(YM3812Init(1, YM3812_RATE, wav_rate))
	{
		printf("Unable to create virtual OPL!!\n");
		return 1;
	}
	printf("OPL created.\n");
	for(int i=1;i<0xf6;i++)
		YM3812Write(0,i,0);

	YM3812Write(0,1,0x20); // Set WSE=1
	//YM3812Write(0,8,0); // Set CSM=0 & SEL=0           // already set in for statement

	outwav.clear();

	if(datalen)
	{
		UINT32 size, cnt=0, imfsize=0xFFFFFFFF;
		char has_notes=0;

		UINT16 rate = 0, fsize = 0;
//		rate = ReadInt16LE(&imfdata);
		fsize = ReadInt16LE(&imfdata);
		datalen -= 2;
		fsize = datalen>>1;/*
		if (fsize){
			imfsize = fsize >> 1;
		}*/
		if (rate != 0 && imf_rate == DEFAULT_IMF_RATE) {
			imf_rate = rate;
			samples_per_tick=wav_rate/imf_rate;
		}
		
		printf("IMF rate is %u Hz.\n", imf_rate);

		buffer = (INT16 *)malloc(samples_per_tick*sizeof(INT16));
		if (buffer)
		{
			printf("buffer allocated.\n");

			UINT16 imfdelay, imfcommand;
			
			printf("Converting IMF data to PCM data\n");

			//write converted PCM data:
			while( (datalen > 0) && imfsize)
			{
				if (isKMF)
				{
					if (!cmds)
					{
						cmds = ReadByte(&imfdata); 
						ticks = ReadByte(&imfdata); 
						datalen -= 2;
						imfsize--;
					}
					if (cmds)
					{
						cmds--;
						ReadData(&imfcommand, sizeof(imfcommand), 1, &imfdata);
						datalen -= sizeof(imfcommand);
						imfsize--;
					}
					if (!cmds)
						imfdelay = ticks;
					else
						imfdelay = 0;
				}
				else
				{
					imfsize--;
					ReadData(&imfcommand, sizeof(imfcommand), 1, &imfdata);
					ReadData(&imfdelay, sizeof(imfdelay), 1, &imfdata);
					datalen -= sizeof(imfcommand);
					datalen -= sizeof(imfdelay);
				}
				switch (imfcommand & 0xFF)
				{
				case 0xB0:
				case 0xB1:
				case 0xB2:
				case 0xB3:
				case 0xB4:
				case 0xB5:
				case 0xB6:
				case 0xB7:
				case 0xB8:
					if (!(channel_mask & (1 << ((imfcommand & 0xFF)-0xB0)))) {
						imfcommand &= 0xDFFF;	//clear KeyOn bit
					} else {
						has_notes = (has_notes || (imfcommand & 0x2000));
					}
					break;
				case 0xBD:
					imfcommand &= ((channel_mask >> 1) & 0x1F00) | 0xE0FF;
					has_notes = (has_notes || (imfcommand & 0x1F00));
					break;
				}
				YM3812Write(0, imfcommand & 0xFF, (imfcommand >> 8) & 0xFF);
				while(imfdelay--)
				{
					YM3812UpdateOne(0, buffer, samples_per_tick);
					for(int ii = 0; ii < samples_per_tick; ii ++){
						outwav.push_back(buffer[ii]);
					}
					if (datalen <= 0) break;
				}
				if(!(cnt++ & 0xff))
				{
					printf(".");
					fflush(stdout);
				}
			}
			printf(" done!\n");

			if (!has_notes)
			{
				printf("Note: The song did not play any notes.\n");
			}
		} else {
			printf("ERROR: out of memory\n");
		}
	} else {
		printf("ERROR: No IMF data!");
	}

	YM3812Shutdown();
	return 0;
}

int ConvertAdSndToBin(std::vector<uint16_t> &outwav, 
					uint8_t *snddata, 
					long datalen, 
					unsigned char instruments[], 
					unsigned char block,
					unsigned int imf_rate,
					unsigned int wav_rate )
{
	unsigned int samples_per_tick = 0;
	INT16 *buffer;
	int cnt = 0;
	
	samples_per_tick=wav_rate/imf_rate;

	if(YM3812Init(1, YM3812_RATE, wav_rate)) {
		printf("Unable to create virtual OPL!!\n");
		return 1;
	}
	printf("OPL created.\n");

	buffer = (INT16 *)malloc(samples_per_tick*sizeof(INT16));
	if (buffer){
		printf("buffer allocated.\n");
		
		printf("Converting IMF data to PCM data\n");


		for(int i=1;i<0xf6;i++)
			YM3812Write(0,i,0);

		YM3812Write(0,1,0x20); // Set WSE=1
		//YM3812Write(0,8,0); // Set CSM=0 & SEL=0           // already set in for statement

		// Setup the instruments
		YM3812Write(0, 0x20, instruments[0]);
		YM3812Write(0, 0x23, instruments[1]);
		YM3812Write(0, 0x40, instruments[2]);
		YM3812Write(0, 0x43, instruments[3]);
		YM3812Write(0, 0x60, instruments[4]);
		YM3812Write(0, 0x63, instruments[5]);
		YM3812Write(0, 0x80, instruments[6]);
		YM3812Write(0, 0x83, instruments[7]);
		YM3812Write(0, 0xE0, instruments[8]);
		YM3812Write(0, 0xE3, instruments[9]);
		// Unused???
		// Makes the sounds muted
		//YM3812Write(0, 0xC0, instruments[10]);
		
		YM3812Write(0, 0xB0, 0);

		unsigned char adblock = (block & 7) << 2;
		bool noteon = false;
		uint8_t oldbyte = 0;

		while (datalen) {
			uint8_t adbyte = *snddata++; 
			if(adbyte==0x00){
				YM3812Write(0, 0xB0, adblock);
				noteon = false;
			}else if(oldbyte != adbyte){
				YM3812Write(0, 0xA0, adbyte);
				if(!noteon){
					YM3812Write(0, 0xB0, adblock | 0x20);
					noteon = true;
				}
			}
			oldbyte = adbyte;

			datalen--;
			
			UINT16 tickdelay = 1;// 140; // What???
			while(tickdelay--){
				YM3812UpdateOne(0, buffer, samples_per_tick);
				for(int ii = 0; ii < samples_per_tick; ii ++){
					outwav.push_back(buffer[ii]);
				}
				if (datalen <= 0) break;
			}

			if(!(cnt++ & 0xff))
			{
				printf(".");
				fflush(stdout);
			}

		}
	}else{
		std::cout << "ERROR! out of memory!" << std::endl;
	}

	YM3812Shutdown();
	return 0;
};

int ConvertRAWToWave(std::string wavfilename, 
						std::vector<uint16_t> &rawdata,
						unsigned int wav_rate)
{
	FILE *out;

	out = fopen(wavfilename.c_str(), "wb");
	if(out) {
		printf("%s opened for output.\n", wavfilename.c_str());

		//write dummy wave header:
		fwrite(&head, sizeof(head), 1, out);
		fwrite(rawdata.data(), sizeof(INT16), rawdata.size(), out);
		fflush(out);

		long size = ftell(out);

		//fill header with correct values:
		head.dSize = size-sizeof(head);
		head.rSize = size-8;
		head.fHertz = wav_rate;
		head.fBlockAlign = head.fChannels*(head.fBits/8);
		head.fBytesPerSec = head.fBlockAlign*wav_rate;

		//write real wave header:
		fseek(out, 0, SEEK_SET);
		fwrite(&head, sizeof(head), 1, out);	
		fflush(out);
		fclose(out);

	} else {
		printf("ERROR: could not write %s\n", wavfilename.c_str());
	}

	return 0;
}
