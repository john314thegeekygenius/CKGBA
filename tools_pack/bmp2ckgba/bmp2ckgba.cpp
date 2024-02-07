/*
 * bmp2ckgba - Written by John314
 * Modified from bmp2ckgba by John314
 * 
 * Compile: g++ bmp2ckgba.cpp  -o bmp2ckgba
 * 
 * Use KEENGRAPH with -tra flag for images
 * 
*/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned char RGB2PAL(unsigned char &colorid){
	// fix here
	if(colorid < 5){ colorid += 1; } // Replace purple with dark red
	if(colorid >= 16) { colorid = 0; } // Transparent at >= 16
	return colorid;
};

int main(int argc, char *args[]){

	if(argc < 2){
		std::cout << "Error! Bad number of args!" << std::endl;
		std::cout << "Use: bmp2ckgba image.bmp <exportdir>" << std::endl;
		return 0;
	}

	std::string imgname;
	int fnamestart = 0;
	for(int i = strlen(args[1])-1; i >= 0; i--){
		if(args[1][i] == '/') {
			fnamestart = i+1;
			break;
		}
	}
	for(int i = fnamestart; i < strlen(args[1]); i++){
		if(args[1][i] == '.') break;
		imgname.push_back(args[1][i]);
	}
	
	std::ifstream BMPFile;
	BMPFile.open(args[1]);
	if(!BMPFile.is_open()){
		std::cerr << "Could not open Bitmap file: " << args[1] << std::endl;
		return 1;
	}
	char hash[2];
	BMPFile.read((char*)hash,2);
	if(hash[0] != 'B' || hash[1] != 'M'){
		std::cerr << args[1] << " is not a valid bitmap file" << std::endl;
		return 1;
	}
	std::cout << "Loading " << args[1] << std::endl;
	// Get pixels location
	BMPFile.seekg(0xA,std::ios_base::beg);
	uint32_t pixeldata;
	BMPFile.read((char*)&pixeldata,4);
	std::cout << "Offset at: 0x" << std::hex << pixeldata << std::dec << std::endl;
	// Skip to size info
	BMPFile.seekg(0x12,std::ios_base::beg);
	uint32_t bmpwidth;
	BMPFile.read((char*)&bmpwidth,4);
	uint32_t bmpheight;
	BMPFile.read((char*)&bmpheight,4);
	// Get the BPP
	uint16_t bmpBPP;
	BMPFile.seekg(0x1C,std::ios_base::beg);
	BMPFile.read((char*)&bmpBPP,2);
	std::cout << "Bitmap BPP is: " << bmpBPP << std::endl;
	// Get the palette size
	uint32_t bmpPalSize;
	BMPFile.seekg(0x2E,std::ios_base::beg);
	BMPFile.read((char*)&bmpPalSize,4);
	if(bmpPalSize == 0){
		bmpPalSize = 1<<(bmpBPP);
	}
	std::cout << "palette size:" << bmpPalSize << std::endl;
	// Skip to pixels
	BMPFile.seekg(pixeldata,std::ios_base::beg);
	
	std::vector<unsigned char> bmppixels;
	bmppixels.resize(bmpheight*bmpwidth);
	for(int y = bmpheight-1; y >= 0; y--){
		for(int x = 0; x < bmpwidth; x++){
			if(bmpBPP == 8){
				BMPFile.read((char*)&bmppixels[(y*bmpwidth)+x],1);
			}
			if(bmpBPP == 4){
				unsigned char byte;
				BMPFile.read((char*)&byte,1);
				bmppixels[(y*bmpwidth)+x] = byte>>4;
				bmppixels[(y*bmpwidth)+x+1] = byte&0xF;
				x++;
			}
		}
	}
	
	BMPFile.close();
	
	std::cout << "Bitmap is " << bmpwidth << "x" << bmpheight << std::endl;
	
	std::vector<unsigned char> gbadata;
	
	int imgWidth = bmpwidth/2;
	int imgHeight = bmpheight;
	
	gbadata.resize(imgWidth*imgHeight);
	
	int pixelpos = 0;
	
	// Swap the pixels around to 8x8 chunks
	for(int e = 0; e < bmpheight; e+=8){
		for(int i = 0; i < bmpwidth; i+=8){
			for(int py = 0; py < 8; py++){
				for(int px = 0; px < 8; px+=2){
					unsigned char pd = RGB2PAL(bmppixels[((e+py)*bmpwidth) + i+px+1]);
					pd <<= 4;
					pd |=  RGB2PAL(bmppixels[((e+py)*bmpwidth) + i+px]);
					gbadata.at(pixelpos) = pd;
					pixelpos+=1;
				}
			}
		}
	}
		
	std::ofstream outputFile;
	std::string openimgname = ""; 
	if(argc > 2){
		openimgname = args[2];
	}
	openimgname += imgname+".c";
	
	outputFile.open(openimgname);
	if(!outputFile.is_open()){
		std::cerr << "Could not open file: " << openimgname << std::endl;
		return 1;
	}
	std::string gbaname;
	int offset = 0;
	for(int i = 0; i < strlen(args[1]); i++){
		if(args[1][i] == '/'){
			offset = i+1;
		}
	}
	for(int i = offset; i < strlen(args[1]); i++){
		if(args[1][i] == '.') break;
		gbaname.push_back(args[1][i]);
	}
	
	if(outputFile.is_open()){
		std::cout << "Saving to " << openimgname << std::endl;
		outputFile << "// Made with png2gba by John314\n";
		outputFile << "//\n";
		outputFile << "const unsigned int " << gbaname << "_width = " << (imgWidth) << ";\n";
		outputFile << "const unsigned int " << gbaname << "_height = " << (imgHeight) << ";\n";
		outputFile << "const unsigned int " << gbaname << "_size = " << (gbadata.size()) << ";\n";
		outputFile << "const unsigned char " << gbaname << "[] = {";
		for(int i = 0; i < gbadata.size(); i++){
			if((i%imgWidth)==0){
				outputFile << "\n\t";
			}
			outputFile << "0x" << std::hex << (int)gbadata.at(i);
			if(i < (gbadata.size()-1)){
				outputFile << ", ";
			}
		}
		outputFile << "\n};\n";
		outputFile.close();
	}

	return 0;
};

