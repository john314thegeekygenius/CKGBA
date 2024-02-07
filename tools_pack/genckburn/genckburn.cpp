// Compile: g++ genckburn.cpp -o genckburn

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <math.h> 
#include <cstdlib>

uint8_t burnBits[160][256];

int main(int argc, char *args[]){
	float burnSeed = 0;
	
	if(argc > 1){
		if(strcmp(args[1],"--help")){
			std::cout << "Use: genckburn <seed>" << std::endl;
		}else{
			burnSeed = atof(args[1]);
		}
	}
	
	srand(burnSeed);

	/*
	 * For every line, generate all lines above untill clear
	 * 
	 * Ex - Each line [x)] is the same line at a different "step":
	 * 
	 * 1) #######################
	 * 2) ## ## ##  # ### ## ### 
	 * 3)  # ##  #  #   # #  #   
	 * 4)     #           #  #  
	 * 5)                        
	 * 
	 * */

	std::cout << "Generating CK Burning patterns" << std::endl;

	// Clear the bits to 1 because that's black in the GBA engine
	memset(&burnBits,1,sizeof(burnBits));
	
	// Open a file to write info to
	std::ofstream bitFile;
	bitFile.open("CK_BURNBITS.c");
	if(!bitFile.is_open()){
		std::cerr << "Failed to open output file: CK_BURNBITS.c" << std::endl;
		return 1;
	}
	
	int BitLineHeight = 32;
	
	// Write some preliminary stuff
	bitFile << "// Generated from genckburn by Joh314\n";
	bitFile << "const unsigned short CK_BURNBITS_LINEHEIGHT = " << (int)BitLineHeight << ";\n";
	bitFile << "const unsigned char CK_BURNBITS[] = {\n";

	for(int burnLine = 0; burnLine < 40; burnLine++){
		std::cout << ".";
		
		for(int bitLine = 0; bitLine < BitLineHeight-1; bitLine++){
			// Write code
			bitFile << "\t";
			// Write the bits out :P
			for(int bit = 0; bit < 256; bit+=2){
				// Make it fast to read out
				uint8_t byte = (burnBits[burnLine][bit]<<4) | (burnBits[burnLine][bit+1]);
				bitFile << "0x" << std::hex << (int)byte << ", ";
			}
			bitFile << "\n";
			// Remove bits
			for(int bc = 0; bc < bitLine; bc++){
				// Find the next available bit
				int bitreplace = rand()%256;
				int bitswap = 1;
				int orginplace = bitreplace;
				while(burnBits[burnLine][bitreplace] == 0){
					bitreplace += bitswap;
					if(bitreplace == 256){
						bitreplace = orginplace;
						bitswap = -1;
					}
					if(bitreplace < 0) { bitreplace = 0; break;}
				}
				burnBits[burnLine][bitreplace] = 0; // Clear that bit
			}
		}
		// Add a final clear line
		bitFile << "\t";
		for(int bit = 0; bit < 256; bit+=2){
			bitFile << "0x0, ";
		}
		bitFile << "\n\n";
	}
	
	bitFile << "};\n";

	// Close the file
	bitFile.close();

	std::cout << std::endl << "Done! Thank you for using genckburn!" << std::endl;
	return 0;
};

