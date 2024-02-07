// ckmap2gba - convert Commander Keen level files to a usefull format
//
// Compile: g++ -o ckmap2gba ckmap2gba.cpp
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



auto GETBYTE(const uint8_t *offset) -> uint8_t
{
    return *((uint8_t *)(offset));
}



auto GETWORD(const uint8_t *ptr) -> uint16_t
{
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//    return (ptr[0]<<8 | ptr[1]);
//#else
    return (ptr[0] | ptr[1] << 8);
//#endif
}

/**
 * @brief READWORD Like GETWORD but the pointer will also be modified, by
 *                 going two bytes forward
 * @param ptr      reference to pointer (*&) which will be modified
 * @return         the value
 */
auto READWORD(uint8_t *&ptr) -> uint16_t
{
    const auto val = GETWORD(ptr);
    ptr += sizeof(uint16_t);
    return val;
}


#define WORDSIZE    2

#define CARMACK_NEARTAG 0xA700
#define CARMACK_FARTAG 0xA800

static void CarmackExpand(void *src, void *dest, int expLength)
{
    uint16_t *srcptr = (uint16_t *)src;
    uint16_t *dstptr = (uint16_t *)dest;
    uint16_t *runptr;
    uint16_t ch, count, offset;
    expLength /= WORDSIZE; // Number of words to deal with

    srcptr++;

    while (expLength > 0)
    {
        ch = GETWORD(reinterpret_cast<uint8_t*>(srcptr));
        srcptr++;

        const auto tag = ch & 0xFF00;
        count = ch & 0xFF;

        if (tag == CARMACK_NEARTAG)
        {
            if (!count)
            {
                // Makes an output starting with A7
                ch &= 0xFF00;
                ch |= GETBYTE(reinterpret_cast<uint8_t*>(srcptr));

                uint8_t *srcptr_8 = (uint8_t *)(srcptr) + 1;
                memcpy(&srcptr, &srcptr_8, sizeof(uint8_t *));

                *(dstptr++) = ch;
                expLength--;
            }
            else
            {
                offset = GETBYTE(reinterpret_cast<uint8_t*>(srcptr));

                uint8_t *srcptr_8 = (uint8_t *)(srcptr) + 1;
                memcpy(&srcptr, &srcptr_8, sizeof(uint8_t *));

                runptr = dstptr - offset;
                expLength -= count;
                while (count--)
                    *(dstptr++) = *(runptr++);
            }
        }
        else if (tag == CARMACK_FARTAG)
        {
            if (!count)
            {
                // Makes an output starting with A8
                ch &= 0xFF00;
                ch |= GETBYTE(reinterpret_cast<uint8_t*>(srcptr));

                uint8_t *srcptr_8 = (uint8_t *)(srcptr) + 1;
                memcpy(&srcptr, &srcptr_8, sizeof(uint8_t *));

                *(dstptr++) = ch;
                expLength--;
            }
            else
            {
                offset = GETWORD(reinterpret_cast<uint8_t*>(srcptr));
                srcptr++;
                runptr = (uint16_t *)dest + offset;;
                expLength -= count;
                while (count--)
                    *(dstptr++) = *(runptr++);
            }
        }
        else
        {
            *(dstptr++) = ch;
            --expLength;
        }
    }
}

/**
 * \brief			This function expands data using carmack
 * 					decompression algorithm
 * \param	source	pointer to the data that is compressed
 * \param	dest	pointer to the data that will be the output
 * \param	length	length of the EXPANDED data
 */
void CCarmack_expand( std::vector<uint8_t>& dst,
                       std::vector<uint8_t>& src,
                       const size_t decarmacksize)
{
    dst.assign(decarmacksize*2, 0);
    void *srcRaw = src.data();
    void *destRaw = dst.data();

    CarmackExpand(srcRaw, destRaw, decarmacksize);

    for(decltype(dst.size()) i=0 ; i<dst.size() ; i+=2)
    {
        const auto temp = dst[i];
        dst[i] = dst[i+1];
        dst[i+1] = temp;
    }
}


#define NEARTAG     0xA7
#define FARTAG      0xA8
#define WORDSIZE    2

#define COUNT       i
#define TAG         i+1
#define OFFSET      i+2

#define OFFSET_MSB  i+3 // Fartag Offset
#define OFFSET_LSB  i+2

#define COPY_BYTE1  j
#define COPY_BYTE2  j+1

void CRLE_expandSwapped( std::vector<uint16_t>& dst, std::vector<uint8_t>& src, uint16_t key )
{
    uint16_t word, count, inc;

    std::size_t finsize;
    uint8_t high_byte, low_byte;

    low_byte = src.at(1);
    high_byte = src.at(0);
    finsize = (high_byte<<8) | low_byte;
    finsize /= 2;

    for(std::size_t i=WORDSIZE ; dst.size() < finsize ; i+=inc)
    {
        if(i+1 >= src.size())
        {
            word = 0;
        }
        else
        {
            // Read datum (word)
            word = (src.at(i)<<8)+src.at(i+1);
        }

        // If datum is 0xFEFE/0xABCD Then
        if( word == key )
        {
            // Read count (word)
            count = (src.at(i+2)<<8)+src.at(i+3);
            word = (src.at(i+4)<<8)+src.at(i+5);

            // Do count times
            for(std::size_t j=0;j<count;j++)
                dst.push_back(word);

            inc = 3*WORDSIZE;
        }
        else
        {
            // Write datum (word)
            dst.push_back(word);
            inc = WORDSIZE;
        }
    }
};

////////////////////////////////////////////////////////




//const uint16_t CK_MAGIC_HASH = 0xABCD;

struct MapHeader {
	uint16_t magic; // should be CK_MAGIC_HASH
	uint32_t ptrs[100]; // pointers to start of level data
						// DNE when 0xFFFFFFFF (-1)
	// Optional tileinfo somewhere

	void read(std::ifstream &file){
		// Read the magic
		file.read((char*)&magic,sizeof(uint16_t));
		std::cout << "Got magic code: 0x" << std::hex << magic << std::dec << std::endl;
		// Read all the pointers
		for(int i = 0; i < 100; i++){
			file.read((char*)&ptrs[i],sizeof(uint32_t));
		}
	};
};

struct LevelHeader {
	uint32_t offPlane[3]; // Offset to beginning of compressed plane, <=0 if no plane present
	uint16_t lenPlane[3]; // Length of compressed plane 0 data (in bytes)
	uint16_t width; // Width of level in tiles
	uint16_t height; // Height of level in tiles
	char name[16]; // Internal name for level, null terminated

	std::vector<uint8_t> CARcompressedData; // Compressed level data
	std::vector<uint8_t> RLEWcompressedData; // Compressed level data ptr2
	std::vector<uint16_t> planeData[3]; // Uncompressed level data
	
	void read(std::ifstream &file){
		// Read all three plane offsets
		file.read((char*)&offPlane,sizeof(uint32_t)*3);
		// Read all the lengths
		file.read((char*)&lenPlane,sizeof(uint16_t)*3);
		// Read the width and height
		file.read((char*)&width,sizeof(uint16_t));
		file.read((char*)&height,sizeof(uint16_t));
		// Read the name
		file.read((char*)&name,sizeof(char)*16);
	};
	void loadplane(std::ifstream &file, int id){
		// Load the compressed data
		CARcompressedData.clear();
		RLEWcompressedData.clear();
		CARcompressedData.resize(lenPlane[id]);
		file.seekg(offPlane[id], std::ios_base::beg);
		std::cout << "Compressed plane " << id << " length:" << lenPlane[id] << std::endl;
		std::cout << "Offset at 0x" << std::hex << offPlane[id] << std::dec << std::endl;
		for(int i = 0; i < CARcompressedData.size(); i++){
			uint8_t byte;
			file.read((char*)&byte,sizeof(char));	
			CARcompressedData.at(i) = byte;
		}
	};
	
};

void CK_CarmackDecompress(std::vector<uint8_t> &uncomp, std::vector<uint8_t> &comp){
	int comp_loc = 0;
	if(!comp.size()) {
		std::cout << "Warn! No compressed data!" << std::endl;
		return;
	}
	// Get the expected size
	uint16_t expectedSize = comp[0] | (comp[1]<<8);
	uint16_t expectedBytes = expectedSize;
	comp_loc += 2;

	expectedSize /= 2; // Divide by word size

	std::cout << "Expecting " << expectedSize << " words" << std::endl;

	uncomp.clear();
	int copycount = 0;

	while( expectedSize){
		uint8_t count = comp[comp_loc++];
		uint8_t flag = comp[comp_loc++];
		if( flag == 0xA7){
			if(count == 0){ // escape
				uint8_t next = comp[comp_loc++];
				uncomp.push_back(next); // Low
				uncomp.push_back(flag); // High
				expectedSize -= 1;
			}else{
				// Near pointer
				uint8_t dist = comp[comp_loc++];
				uint32_t copyloc = uncomp.size() - ((int)dist);
				for(int i = 0; i < count*2; i++){
					uint8_t cpb = uncomp.at(copyloc+i);
					uncomp.push_back(cpb);
				}
				expectedSize -= count;
			}
		} else if(flag == 0xA8) {
			if(count == 0){ // escape
				uint8_t next = comp[comp_loc++];
				uncomp.push_back(next); // Low
				uncomp.push_back(flag); // High
				expectedSize -= 1;
			}else{
				// Far pointer
				uint16_t copyloc = comp[comp_loc++];
						copyloc |= comp[comp_loc++]<<8;
				std::cout << "CAR_Copy Location: " << copyloc << std::endl;
				copycount++;
				for(int i = 0; i < count*2; i++){
					uint8_t cpb = uncomp.at(copyloc+i);
					uncomp.push_back(cpb);
				}
				expectedSize -= count;
			}
		}else{
			uncomp.push_back(count);
			uncomp.push_back(flag);
			expectedSize -= 1;
		}
	}
	if(comp_loc < comp.size()){
		std::cout << "NOT ALL BYTES COPIED!" << std::endl;
	}
	std::cout << "Copy Count:" << std::dec << copycount << std::endl;
	if(expectedBytes != uncomp.size()){
		std::cout << "ERROR! Carmack expected size is not same as uncompressed!" << std::endl;
		std::cout << "Got: " << uncomp.size() << " bytes, expected:" << expectedBytes << " bytes" << std::endl;
		return;
	}
	
	if(!uncomp.size()){
		std::cout << "Warn! No data uncompressed!" << std::endl;
	}
	std::cout << "Carmack done... size of:" << uncomp.size() << std::endl;
};


void CK_RLEWDecompress(std::vector<uint16_t> &uncomp, std::vector<uint8_t> &comp, uint16_t MAGIC){
	if(!comp.size()) {
		std::cout << "Warn! No compressed data!" << std::endl;
		return;
	}
	uint16_t expectedSize = comp.at(0) | (comp.at(1)<<8);
	uint16_t expectedBytes = expectedSize;
	
	expectedSize /= 2; // divide by word size
	std::cout << "Expecting " << expectedSize << " words" << std::endl;
	
	// Skip the first 2 bytes
	int inc = 0;
	uint16_t readword = 0;
	
	uncomp.clear();
	
	//uncomp.size() < expectedSize;
	for(int ptri = 2; ptri < comp.size(); ptri += inc){
		if(ptri+1 >= comp.size()){
            readword = 0;
            std::cout << "Trailing byte!" << std::endl;
        } else {
            // Read datum (word)
            readword = comp.at(ptri) | (comp.at(ptri+1)<<8);
        }
		if(readword == MAGIC){
			// Read count (word)
			uint16_t Count = comp.at(ptri+2) | (comp.at(ptri+3)<<8);
			// Read value
			uint16_t Value = comp.at(ptri+4) | (comp.at(ptri+5)<<8);
			std::cout << "Magic @ 0x" << std::hex << ptri << "! Writting " << std::dec << Count << " words of 0x" << std::hex << Value << std::dec << std::endl;
			if(Value == MAGIC){
				// Ummm...
				break;
			}else{
				for(int i = 0; i < Count; i++){
					uncomp.push_back(Value);
					if(uncomp.size() == expectedSize){
						std::cout << "Error at MAGIC word" << std::endl;
						std::cout << "Count: " << i << std::endl;
//						return;
					}
				}
			}
            inc = 6;
		}else{
			uncomp.push_back(readword);
			inc = 2;
		}
	}

	if(expectedSize != uncomp.size()){
		std::cout << "ERROR! RLEW expected size is not same as uncompressed!" << std::endl;
		std::cout << "Got: " << (uncomp.size()*2) << " bytes, expected:" << expectedBytes << " bytes" << std::endl;
		return;
	}
		
	if(!uncomp.size()){
		std::cout << "Warn! No data uncompressed!" << std::endl;
	}
	std::cout << "RLEW done... size of " << uncomp.size() << std::endl;
};



int main(int argc, char *args[]){
	MapHeader MAPHEAD;
	LevelHeader GAMEMAPS[100];
	
	if(argc < 3){
		std::cout << "Invalid number of arguments!" << std::endl;
		std::cout << "Use: ckmap2gba maphead.ckX gamemaps.ckX" << std::endl;
		return 0;
	}
	
	std::ifstream loadfile;
	std::ofstream headerfile;
	loadfile.open(args[1]);
	if(loadfile.is_open()){
		std::cout << "Loading mapheader: " << args[1] << std::endl;
		MAPHEAD.read(loadfile);
		loadfile.close();
		// Makesure it's a valid header
/*		if(MAPHEAD.magic != CK_MAGIC_HASH){
			std::cout << "Error! Invalid Map Header!" << std::endl;
			return 0;
		}*/
	}
	loadfile.open(args[2]);
	if(loadfile.is_open()){
		std::cout << "Loading gamemaps: " << args[2] << std::endl;
		for(int i = 0; i < 100; i++){
			if((signed)MAPHEAD.ptrs[i] <= 0){
				continue; // Invalid map
			}
			// Seek to location
			loadfile.seekg(MAPHEAD.ptrs[i], std::ios_base::beg);
			// Read the header
			GAMEMAPS[i].read(loadfile);

			if(GAMEMAPS[i].width == 0 || GAMEMAPS[i].height == 0){
				continue; // Ignore it
			}
			// Print the data
			std::cout << "Loading " << GAMEMAPS[i].name << std::endl;
			std::cout << "--Map size:" << GAMEMAPS[i].width << "x" << GAMEMAPS[i].height << std::endl;
			std::cout << "--Map loc :" << MAPHEAD.ptrs[i] << std::endl;

			for( int planeid = 0; planeid < 3; planeid++){
				GAMEMAPS[i].loadplane(loadfile, planeid);
				/*
				std::ofstream carmak;
				carmak.open("RAW"+std::string(GAMEMAPS[i].name)+".dmp");
				carmak.write((char*)GAMEMAPS[i].CARcompressedData.data(), GAMEMAPS[i].CARcompressedData.size());
				carmak.close();*/
				CCarmack_expand(GAMEMAPS[i].RLEWcompressedData, GAMEMAPS[i].CARcompressedData, GAMEMAPS[i].CARcompressedData.size()*2);
//				CK_CarmackDecompress(GAMEMAPS[i].RLEWcompressedData, GAMEMAPS[i].CARcompressedData);
		/*		
				carmak.open("CAR"+std::string(GAMEMAPS[i].name)+".dmp");
				carmak.write((char*)GAMEMAPS[i].RLEWcompressedData.data(), GAMEMAPS[i].RLEWcompressedData.size());
				carmak.close();
	*/			
				GAMEMAPS[i].planeData[planeid].clear();
				CRLE_expandSwapped(GAMEMAPS[i].planeData[planeid], GAMEMAPS[i].RLEWcompressedData, MAPHEAD.magic);
				//CK_RLEWDecompress(GAMEMAPS[i].planeData[planeid], GAMEMAPS[i].RLEWcompressedData, MAPHEAD.magic);
/*
				carmak.open("RLE"+std::string(GAMEMAPS[i].name)+".dmp");
				carmak.write((char*)GAMEMAPS[i].planeData[planeid].data(), GAMEMAPS[i].planeData[planeid].size()*2);
				carmak.close();*/

				if(GAMEMAPS[i].planeData[planeid].size() != GAMEMAPS[i].width*GAMEMAPS[i].height){
					std::cout << "Uhoh! Did not extract data correctly!" << std::endl;
					std::cout << "Got:" << GAMEMAPS[i].planeData[planeid].size() << " - Should have gotten:" << (GAMEMAPS[i].width*GAMEMAPS[i].height) << std::endl;
					loadfile.close();
					return 0;
				}
			}
		}
		loadfile.close();
	}
	
	headerfile.open("CKx_LDefs.h");
	if(!headerfile.is_open()){
		std::cout << "Failed to open header!" << std::endl;
		return 1;
	}
	
	headerfile << "// Level definitions generated by ckmap2gba\n\n";
	headerfile << "#ifndef __CKx_LDEFS__\n";
	headerfile << "#define __CKx_LDEFS__\n\n";
	headerfile << "// Include all the levels\n";

	// Decompress the levels
	std::cout << "Decompressing levels..." << std::endl;

	int levelId = 0;

	for(int i = 0; i < 100; i++){
		if(MAPHEAD.ptrs[i] == 0xFFFFFFFF || MAPHEAD.ptrs[i] == 0){
			continue; // Invalid map
		}
		if(GAMEMAPS[i].width == 0 || GAMEMAPS[i].height == 0){
			continue; // Ignore it
		}
		
		// Write to a file
		std::string convertedName = "L"+std::to_string(levelId)+"_";
		for(int m = 0; m < strlen(GAMEMAPS[i].name); m++){
			char ch = toupper(GAMEMAPS[i].name[m]);
			if(!isalpha(ch) || ch == ' '){
				ch = '_';
			}
			convertedName.push_back(ch);
		}
		std::cout << "New level name: " << convertedName << std::endl;
		if(convertedName.length()){
			std::ofstream outputFile;
			outputFile.open("export/"+convertedName+".c");
			if(outputFile.is_open()){
				std::cout << "Writting to " << "export/"+convertedName+".c" << std::endl;
				outputFile << "// Made with ckmap2gba by John314\n";
				outputFile << "//\n";
				outputFile << "const unsigned short " << convertedName << "_width = " << (int)GAMEMAPS[i].width << ";\n";
				outputFile << "const unsigned short " << convertedName << "_height = " << (int)GAMEMAPS[i].height << ";\n";
				outputFile << "const unsigned short " << convertedName << "_data[] = {\n";
				for( int planeid = 0; planeid < 3; planeid++){
					// Only write valid data ????
					for(int bi = 0; bi < GAMEMAPS[i].width*GAMEMAPS[i].height; bi++){
						if((bi % GAMEMAPS[i].width) == 0){
							outputFile << "\n\t";
						}
						outputFile << "0x" << std::hex << std::setw(4) << std::setfill('0')  << (int)GAMEMAPS[i].planeData[planeid].at(bi);
						if(planeid == 3){
							if(bi < GAMEMAPS[i].planeData[planeid].size()-1){
								outputFile << ", ";
							}
						}else{
							outputFile << ", ";
						}
					}
					outputFile << "\n";
				}
				outputFile << "\n};\n";
				outputFile.close();
				std::cout << "Done!" << std::endl;
			}
			headerfile << "extern const unsigned short " << convertedName << "_width;\n";
			headerfile << "extern const unsigned short " << convertedName << "_height;\n";
			headerfile << "extern const unsigned short " << convertedName << "_data[];\n";
		}
		levelId++;
	}

	levelId = 0;

	headerfile << "\n\nconst unsigned short * CK_LevelInfo[] = {";
	for(int i = 0; i < 100; i++){
		if(MAPHEAD.ptrs[i] == 0xFFFFFFFF || MAPHEAD.ptrs[i] == 0){
			continue; // Invalid map
		}
		if(GAMEMAPS[i].width == 0 || GAMEMAPS[i].height == 0){
			continue; // Ignore it
		}
		
		// Write to a file
		std::string convertedName = "L"+std::to_string(levelId)+"_";
		for(int m = 0; m < strlen(GAMEMAPS[i].name); m++){
			char ch = toupper(GAMEMAPS[i].name[m]);
			if(!isalpha(ch) || ch == ' '){
				ch = '_';
			}
			convertedName.push_back(ch);
		}
		headerfile << "\n// Level " << levelId << "\n";
		headerfile << "\t&"+convertedName+"_width, &"+convertedName+"_height, " << convertedName << "_data,\n";

		levelId++;
	}
	headerfile << "};\n\n#endif";
	std::cout << "Finished!" << std::endl;
	
	
	return 0;
};

