// Compile: g++ -o tli2gba TLI2GBA.cpp
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <cmath>

std::vector<signed char> ck_info;
const int BGPLANES = 2;
const int FGPLANES = 7;


signed char CK_TileInfo[0xFFFF]; // Tile info of tiles

int str2int(std::string str){
	int v = 0;
	for(char c : str){
		v *= 10;
		v += (c-'0');
	}
	return v;
};

int main(int argc, char *args[]){

	int BGTiles = 0;
	int FGTiles = 0;

	std::vector<std::string> argv;
	for(int i = 0; i < argc; i++)
		argv.push_back(std::string(args[i]));
		
    if(argv.size() < 4){
        std::cout << "Please use: tli2gba info.tli bgtiles fgtiles" << std::endl;
        return 0;
    }

    std::string fname = argv[1];
    
    BGTiles = str2int(argv[2]);
    FGTiles = str2int(argv[3]);

    std::cout << "Opening " << fname << "..." << std::endl;
    

    std::fstream tinfoFile;
	// Open the file
    tinfoFile.open(fname,std::ios::in);

    if(tinfoFile.is_open()){
        bool DoneGetting = false;

        std::cout << "Reading data..." << std::endl;
        
		signed char byteA;

		int p = 0;
		int c = 0;
		// Get all tiles
        while(DoneGetting==false){
            byteA = tinfoFile.get(); // First byte
            if(tinfoFile.eof()){
                std::cout << "Done collecting data!" << std::endl;
                std::cout << "Collected " << ck_info.size() << " bytes!" << std::endl;
                DoneGetting = true;
			}else{
                ck_info.push_back(byteA);
            }
        }

    }else{
        std::cout << "Failed to open " << fname << std::endl;
        return 0;
    }
    tinfoFile.close();
    
    if( (FGTiles*FGPLANES)+(BGTiles*BGPLANES) != ck_info.size() ){
		std::cout << "Wrong number of bytes collected!" << std::endl;
		return 0;
	}

    // Remove the extention name
    std::string nname;
    for(char c : fname){
		if(c == '.'){
			break;
		}
		nname += c;
	}
	fname = nname;
    
	std::cout << "Saving Info" << std::endl;
	fname += "_gba_info.c";
    // Open the file
    tinfoFile.open(fname,std::ios::out);

    if(tinfoFile.is_open()){
		tinfoFile << "const uint16_t CK_TileInfo_BGTiles = " << BGTiles << ";\n" << std::endl;
		tinfoFile << "const uint16_t CK_TileInfo_FGTiles = " << FGTiles << ";\n" << std::endl;
		tinfoFile << "const uint32_t CK_TileInfo_size = " << ck_info.size() << ";\n" << std::endl;
		tinfoFile << "const char CK_TileInfoBG[] = {";
		// Save the tile info
		for(int i = 0; i < BGTiles*BGPLANES; i++){
			if((i%(18*4))==0){
				tinfoFile << "\n\t";
			}
			tinfoFile << "0x" << std::hex << (int)(ck_info[i]&0xFF) << ", ";
		}
		tinfoFile << "\n};" << std::endl;
		tinfoFile << "\n// ::FGTILES:: //\n";
		tinfoFile << "const char CK_TileInfoFG[] = {";
		// Save the tile info
		for(int i = 0; i < FGTiles*FGPLANES; ){
			if((i%(18*4))==0){
				tinfoFile << "\n\t";
			}
			tinfoFile << "0x" << std::hex << (int)(ck_info[(BGTiles*BGPLANES)+i]&0xFF) << ", ";
			i++;
			if( (i % FGTiles) == 0){
				tinfoFile << "\n\n\t";
			}
		}
		tinfoFile << "\n};" << std::endl;
    }else{
        std::cout << "Failed to open " << fname << std::endl;
        return 0;
    }
    tinfoFile.close();
    
	std::cout << "Done converting tile info" << std::endl;
    
    return 0;
};
