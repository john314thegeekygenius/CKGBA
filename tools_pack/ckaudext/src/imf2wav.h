#pragma once


#define YM3812_RATE 3579545
#define DEFAULT_FREQ 44100
#define DEFAULT_IMF_RATE 560

int ConvertIMFToBin(std::vector<uint16_t> &outwav, 
					uint8_t *imfdata, 
					long datalen = 0, 
					unsigned int imf_rate = DEFAULT_IMF_RATE,
					unsigned int wav_rate = DEFAULT_FREQ);

int ConvertAdSndToBin(std::vector<uint16_t> &outwav, 
					uint8_t *snddata, 
					long datalen, 
					unsigned char instruments[], 
					unsigned char block,
					unsigned int imf_rate = 140,
					unsigned int wav_rate = DEFAULT_FREQ);
					
int ConvertRAWToWave(std::string wavfilename,
					std::vector<uint16_t> &rawdata,
					unsigned int wav_rate = DEFAULT_FREQ);
