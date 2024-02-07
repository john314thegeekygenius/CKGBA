// Commander Keen Audio Extractor by John314
// g++ ckaudext.cpp imf2wav.cpp fmopl.cpp -o ckaudext
//
// With imf to wave conversion from K1n9_Duk3's IMF to WAV converter
//
// The OPL emulator (fmopl.cpp, fmopl.h) is used under the terms of the
// MAME license (see license-mame.txt for more details). 
//

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <math.h> 

#include "imf2wav.h"

#define GBA_INV_11025 0.005412988
#define GBA_INV_22050 0.002709921
#define GBA_INV_44100 0.001353247


const int GBA_WavRate = 11025;

std::string g_AudioName, g_AudioHeadName;
int g_numOfSounds = 0;
int g_startIMFid = 0;

struct ck_audioBlock {
	uint32_t offset;
	uint32_t size;
};

struct ck_pcsound {
	uint32_t length;
	uint16_t priority;
	std::vector<unsigned char> data;
	unsigned char terminator; // ???
};

struct ck_adlibsound {
	uint32_t length;
	uint16_t priority;
	unsigned char instrument[16]; 
	unsigned char block;
	std::vector<unsigned char> data;
	unsigned char terminator; // ???
	std::string name;
	
	long wavsize;
};

struct ck_imf {
//	uint32_t length; // Commonly known as Type-1 IMF
	std::vector<unsigned char> data;
	std::string name;
	std::string info;

	long wavsize;
};

std::vector<ck_audioBlock> g_audioHead;

std::vector<ck_pcsound> g_pcSounds;
std::vector<ck_adlibsound> g_adlibSounds;
std::vector<ck_imf> g_imfSongs;

void LoadAudioHead(){
	std::cout << "Loading audio head..." << std::endl;
	// Load the offsets
	std::ifstream file;
	file.open(g_AudioHeadName);
	if(file.is_open()){
		uint32_t lastOffset = 0;
		struct ck_audioBlock block;
		while(1){
			uint32_t newOffset;
			file.read((char*)&newOffset, 4);
			if(file.eof()){ break; }
			block.offset = newOffset;
			block.size = newOffset - lastOffset;
			lastOffset = newOffset;
			g_audioHead.push_back(block);
			std::cout << "Found offset " << block.offset << std::endl;
		}
		g_audioHead.pop_back();
		std::cout << "Read " << g_audioHead.size() << " audio chunks" << std::endl;
		file.close();
	}else{
		std::cout << "Could not open audio header file: " << g_AudioHeadName << std::endl;
		exit(0);
	}
};

/*
* PC Speaker sound effects
* AdLib sound effects
* Digitized sound effects
* Adlib music chunks
*/

void LoadAudio(){
	std::cout << "Loading audio..." << std::endl;
	// Load the offsets
	std::ifstream file;
	file.open(g_AudioName);
	if(file.is_open()){
		ck_audioBlock block;

		// PC SOUNDS
		for(int sndi = 0; sndi < g_numOfSounds; sndi++){
			block = g_audioHead.at(sndi);
			file.seekg(block.offset);
			// Load PC Sounds
			ck_pcsound tempsound;
			uint32_t chunksize;
			file.read((char*)&chunksize,4);
			file.read((char*)&tempsound.length,4);
			file.read((char*)&tempsound.priority,2);
			for(int e = 0; e < tempsound.length; e++){
				unsigned char d;
				file.read((char*)&d,1);
				tempsound.data.push_back(d);
				if(file.eof()){
					std::cout << "Error loading sound " << sndi << " Loading " << e << " bytes" << std::endl;
					exit(1);
				}
			}
			file.read((char*)&tempsound.terminator,1); // ???
			std::cout << "Loaded " << sndi << " " << tempsound.length << " " << tempsound.priority << " " << std::endl;
			g_pcSounds.push_back(tempsound);
		}
		std::cout << "Read " << g_pcSounds.size() << " PC Sounds" << std::endl;

		// ADLIB SOUNDS
		for(int sndi = 0; sndi < g_numOfSounds; sndi++){
			block = g_audioHead.at(sndi+g_numOfSounds);
			file.seekg(block.offset);
			// Load Adlib Sounds
			ck_adlibsound tempsound;
			uint32_t chunksize;
			file.read((char*)&chunksize,4);
			file.read((char*)&tempsound.length,4);
			file.read((char*)&tempsound.priority,2);
			for(int e = 0; e < 16; e++){
				file.read((char*)&tempsound.instrument[e],1);
			}
			file.read((char*)&tempsound.block,1);

			for(int e = 0; e < tempsound.length; e++){
				unsigned char d;
				file.read((char*)&d,1);
				tempsound.data.push_back(d);
				if(file.eof()){
					std::cout << "Error loading sound " << sndi << " Loading " << e << " bytes" << std::endl;
					exit(1);
				}
			}
			// Load the name
			tempsound.name = "SND_"+std::to_string(sndi+g_numOfSounds);

			std::cout << "Loaded " << tempsound.name << "(" << sndi << ") " << tempsound.length << " " << tempsound.priority << " " << (int)tempsound.block << std::endl;
			g_adlibSounds.push_back(tempsound);
		}
		std::cout << "Read " << g_adlibSounds.size() << " ADLIB Sounds" << std::endl;

		// IMF SONG FILES
		// Note:
		// We don't handle digital sounds
		for(int songi = (g_numOfSounds*2); songi < g_audioHead.size(); songi++){
			block = g_audioHead.at(songi);
			file.seekg(block.offset);
			// Load IMF Songs
			ck_imf tempsong;
			uint32_t chunksize;
			file.read((char*)&chunksize,4);
			for(int e = 0; e < chunksize; e++){
				unsigned char d;
				file.read((char*)&d,1);
				tempsong.data.push_back(d);
				if(file.eof()){
					std::cout << "Error loading IMF " << songi << " Loading " << e << " bytes" << std::endl;
					exit(1);
				}
			}
			// Load the tags (if any)
			unsigned char d;
			file.read((char*)&d,1);
			if(d != 0x1A){
				// Skip the next byte
				file.read((char*)&d,1);
			}
			// Read the name!
			do{
				file.read((char*)&d,1);
				if(d){
					tempsong.name += d;
				}
			}while(d);

			// Fix the name if we read bad data
			if(!isprint(tempsong.name[0]) || tempsong.name.length() < 4){
				tempsong.name = "IMF_"+std::to_string(songi);
			}
		
			std::cout << "Loaded " << tempsong.name << "(" << songi << ") " << tempsong.data.size() << " " << std::endl;
			g_imfSongs.push_back(tempsong);
		}
		std::cout << "Read " << g_imfSongs.size() << " IMF Files" << std::endl;

		file.close();
	}else{
		std::cout << "Could not open audio file: " << g_AudioName << std::endl;
		exit(0);
	}
};

std::string GetFixedName(int sndid){
	std::stringstream ss;	
	ss.str("");
	ss << std::setw(3) << std::setfill('0') << (int)sndid;
	return "SND1_" + ss.str();
};

void ExportRawData(std::string dir, std::string name, std::vector<uint16_t> &wavdata){
	std::ofstream file;

	file.open(dir+name+".c");
	if(file.is_open()){
		std::cout << "Writing " << name << " -- size:" << wavdata.size() << std::endl;
		file << "// IMF WAV data generated by ckaudext by John314\n";
		file << "// \n\n";
		file << "const unsigned int " << name << "_rate = " << std::dec << (int)GBA_WavRate << ";\n";
		file << "//const unsigned int " << name << "_size = " << std::dec << (int)wavdata.size() << ";\n";
		file << "const unsigned char " << name << "_data[] = {";
		// Write the data now
		int modcnt = 0;
		for(uint16_t dw : wavdata){
			if((modcnt%64)==0){
				file << "\n\t";
			}
			// Convert the word into valid bytes
			unsigned char lb =  dw&0xFF;
			unsigned char hb =  dw>>8;
			file << "0x" << std::hex << (int)hb << ", ";
//			file << "0x" << std::hex << (int)hb << ", ";
			modcnt++;
		}
		file << "\n};\n";
		file.close();
	}else{
		std::cout << "Failed to open " << dir << name << ".c" << std::endl;
	}
};

void TruncWav(std::vector<uint16_t> &wavdata){
	float vblanks = wavdata.size()*2*GBA_INV_11025;
	int rvblanks = floor(vblanks);
	std::cout << "VBlank calculation: " << rvblanks << " :: " << vblanks << std::endl;
	if(rvblanks < vblanks){
		int newsize = rvblanks/GBA_INV_11025;
		newsize /= 2;
		int delta = wavdata.size() - newsize;
		if( delta > 0){
			wavdata.resize(wavdata.size()-delta);
			std::cout << "Cut off " << delta << " bytes of audio..." << std::endl;
		}
	}
};

void ExportMusic(){
	std::cout << "Exporting IMFs..." << std::endl;
	
	for(int imfCount = g_startIMFid - (g_numOfSounds*2); imfCount < g_imfSongs.size(); imfCount++){
		ck_imf imfdata = g_imfSongs.at(imfCount);
		std::ofstream file;
		std::string sndName = GetFixedName(imfCount+(g_numOfSounds*2));

		file.open("EXPORT/IMFS/"+sndName+".imf");
		if(file.is_open()){
			file.write((char*)imfdata.data.data(), imfdata.data.size());
			file.close();
		}else{
			std::cout << "Failed to open EXPORT/IMFS/" << sndName << ".imf" << std::endl;
		}

		std::vector<uint16_t> wavdata;

		if(ConvertIMFToBin(wavdata,  
			imfdata.data.data(), 
			imfdata.data.size(), 
			DEFAULT_IMF_RATE, GBA_WavRate) != 0)
		{
			std::cout << "Failed to convert!" << std::endl;
			return;
		}
		
		TruncWav(wavdata);
		
		ExportRawData("EXPORT/MUSIC/", sndName, wavdata);

		g_imfSongs.at(imfCount).wavsize = wavdata.size();

		ConvertRAWToWave("EXPORT/WAVS/"+sndName+".wav", wavdata, GBA_WavRate);

	}
	
};

void ExportSoundsPC(){
	std::cout << "Exporting PC Sounds..." << std::endl;
	int sndCount = 0;
	std::string sndName;
	for(ck_pcsound pcsnd : g_pcSounds){
		std::ofstream file;

		sndName = GetFixedName(sndCount);

		file.open("EXPORT/PC/"+sndName+".c");
		if(file.is_open()){
			std::cout << "Writing " << sndName << " -- size:" << pcsnd.data.size() << std::endl;
			file << "// PC Sound data generated by ckaudext by John314\n";
			file << "// \n\n";
			file << "const unsigned int " << sndName << "_length = " << std::dec << (int)pcsnd.length << ";\n";
			file << "const unsigned short " << sndName << "_priority = " << std::dec << (int)pcsnd.priority << ";\n";
			file << "const unsigned char " << sndName << "_data[] = {";
			// Write the data now
			int modcnt = 0;
			for(unsigned char c : pcsnd.data){
				if((modcnt%64)==0){
					file << "\n\t";
				}
				file << "0x" << std::hex << (int)c << ", ";
				modcnt++;
			}
			file << "\n};\n";
			file.close();
		}else{
			std::cout << "Failed to open EXPORT/PC/" << sndName << ".c" << std::endl;
		}
		sndCount += 1;
	}
};


void ExportSoundsAdlib(){
	std::cout << "Exporting Adlib Sounds..." << std::endl;
	
	std::string sndName;
	for(int sndCount = 0; sndCount < g_adlibSounds.size(); sndCount++){
		ck_adlibsound snddata = g_adlibSounds.at(sndCount);
		std::ofstream file;
		sndName = GetFixedName(sndCount+g_numOfSounds);

		file.open("EXPORT/SIMFS/"+sndName+".imf");
		if(file.is_open()){
			file.write((char*)snddata.data.data(), snddata.data.size());
			file.close();
		}else{
			std::cout << "Failed to open EXPORT/SIMFS/" << sndName << ".imf" << std::endl;
		}

		std::vector<uint16_t> wavdata;

		if(ConvertAdSndToBin(wavdata,
			snddata.data.data(), 
			snddata.data.size(), 
			snddata.instrument,
			snddata.block,
			140, GBA_WavRate) != 0)
		{
			std::cout << "Failed to convert!" << std::endl;
			return;
		}
		
		TruncWav(wavdata);

		g_adlibSounds.at(sndCount).wavsize = wavdata.size();
		
		ExportRawData("EXPORT/ADLIB/", sndName, wavdata);

		ConvertRAWToWave("EXPORT/WAVS/"+sndName+".wav", wavdata, GBA_WavRate);

	}
};

void WriteHeaders(){
	std::cout << "Writting Audio Header Files..." << std::endl;

	std::ofstream file;
	file.open("EXPORT/CKx_MusicDef.h");
	if(file.is_open()){
		file << "// Music Info Header generated by ckaudext by John314\n";
		file << "// \n\n";

		for(int imfCount = g_startIMFid - (g_numOfSounds*2); imfCount < g_imfSongs.size(); imfCount++){
			std::string sndName = GetFixedName(imfCount+(g_numOfSounds*2));
			file << "const unsigned int " << sndName << "_size = " << (int)g_imfSongs.at(imfCount).wavsize << ";\n";
			file << "extern const unsigned char " << sndName << "_data[];\n";
		}

		file << "const GBA_SoundSample CKM_GBA_Samples [] = {";
		for(int imfCount = g_startIMFid - (g_numOfSounds*2); imfCount < g_imfSongs.size(); imfCount++){
			std::string sndName = GetFixedName(imfCount+(g_numOfSounds*2));
			file << "\n\t{&"+sndName+"_data , "+sndName+"_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },";
		}
		file << "\n};\n";
		file.close();
	}else{
		std::cout << "Failed to open EXPORT/CKx_MusicDef.h" << std::endl;
	}
	
	file.open("EXPORT/CKx_PCDefs.h");
	if(file.is_open()){
		file << "// PC Sound definitions generated by ckaudext by John314\n";
		file << "// \n\n";

		for(int sndcnt = 0; sndcnt < g_pcSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt);
			file << "extern const unsigned char "+sndName+"_data[];\n";
			file << "extern const unsigned short "+sndName+"_priority;\n";
			file << "extern const unsigned int "+sndName+"_length;\n";
		}
		file << "\n\n";

		file << "const unsigned char *CKS_PC_Samples [] = {";
		for(int sndcnt = 0; sndcnt < g_pcSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt);
			file << "\n\t&"+sndName+"_data, ";
		}
		file << "\n};\n";
		file << "const unsigned int *CKS_PC_SampleLens [] = {";
		for(int sndcnt = 0; sndcnt < g_pcSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt);
			file << "\n\t&"+sndName+"_length, ";
		}
		file << "\n};\n";
		file << "const unsigned short *CKS_SamplePriorities [] = {";
		for(int sndcnt = 0; sndcnt < g_pcSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt);
			file << "\n\t&"+sndName+"_priority, ";
		}
		file << "\n};\n";
		file.close();
	}else{
		std::cout << "Failed to open EXPORT/CKx_PCDefs.h" << std::endl;
	}
	
	
	file.open("EXPORT/CKx_AdlibDefs.h");
	if(file.is_open()){
		file << "// Adlib Sound definitions generated by ckaudext by John314\n";
		file << "// \n\n";


		for(int sndcnt = 0; sndcnt < g_adlibSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt+g_numOfSounds);
			file << "const unsigned int " << sndName << "_size = " << (int)g_adlibSounds.at(sndcnt).wavsize << ";\n";
			file << "extern const unsigned char " << sndName << "_data[];\n";
		}
		file << "\n\nconst GBA_SoundSample CKS_GBA_Samples [] = {";
		for(int sndcnt = 0; sndcnt < g_adlibSounds.size(); sndcnt++){
			std::string sndName = GetFixedName(sndcnt+g_numOfSounds);
			file << "\n\t{&"+sndName+"_data , "+sndName+"_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },";
		}
		file << "\n};\n";
		file.close();
	}else{
		std::cout << "Failed to open EXPORT/CKx_AdlibDefs.h" << std::endl;
	}
	
	
};

int main(int argc, char *args[]){
	std::cout << "ckaudext - Commander Keen audio extractor for GBA-KEEN" << std::endl;
	if(argc < 5){
		std::cout << "Use: ckaudext AUDIO.CKx AUDIOHED.CKx num_sounds imf_start" << std::endl;
		return 0;
	}
	g_AudioName = args[1];
	g_AudioHeadName = args[2];
	g_numOfSounds = atoi(args[3]);
	g_startIMFid = atoi(args[4]);
	
	LoadAudioHead();
	LoadAudio();
	
	ExportMusic();
	ExportSoundsPC();
	ExportSoundsAdlib();
	
	WriteHeaders();
	
	std::cout << "Done!" << std::endl;
	return 0;
};

