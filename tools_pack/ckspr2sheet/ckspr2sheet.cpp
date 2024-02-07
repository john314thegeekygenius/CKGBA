// ckspr2sheet written by John314
// Uses KeenGraph LIST.TXT file and sprite bitmaps for info
// Compile : g++ -o ckspr2sheet ckspr2sheet.cpp
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
#include <map>


/*

~Sprites:
  id    bmp name   collide rect    offy,offx  shifts 
 124: 4SPR0124.BMP                             4
 129: 4SPR0129.BMP (0,0,63,23)       [0,3]     4

	* If a list entry does not have a collide rect, it is defined in the image
	* Collide rect is defined as: (x0, y0, x1,y1)
	* 
	* (x0,y0)
	*    +-------------+
	*    |             |
	*    |             |
	*    +-------------+ (x1,y1)
	* 
*/
#ifndef MAX_INT
#define MAX_INT 0x7FFF
#endif

const int CK_MAX_SPRITES = 0xFFFF;// Should never have more than 65535 sprites????

struct CK_SprBox {
	int left = 0;
	int top = 0;
	int right = MAX_INT;
	int bottom = MAX_INT;
	// Not for the box
	int offx = MAX_INT;
	int offy = MAX_INT;
//	int shifts; // Useless to us
	bool readbox = false;
	bool readoff = false;
	void clear(){
		left = top = 0;
		right = bottom = MAX_INT;
		offx = offy = MAX_INT;
		readbox = false;
		readoff = false;
	}
};


std::map<int, struct CK_SprBox> CK_SpriteMaps;

int LoadSpriteBoxes(std::string SpriteDir){
	std::ifstream KGListFile;
	// Read all the information first
	KGListFile.open("LIST.TXT");
	if(!KGListFile.is_open()){
		// Check in the spritedir
		KGListFile.open(SpriteDir+"LIST.TXT");
		if(!KGListFile.is_open()){
			std::cout << "Failed to open LIST.TXT!" << std::endl;
			return 1;
		}
	}

	// The list file is open now.
	// So find the right starting line
	std::string testline;
	while(testline.substr(0,9).compare("~Sprites:") != 0){
		std::getline(KGListFile, testline);
		if(KGListFile.eof()) {
			std::cout << "Error! EOF reached! Bad list file!" << std::endl;
			return 1;
		}
	}
	std::cout << "Found sprite list start..." << std::endl;
	std::cout << "Reading sprite info..." << std::endl;
	// Skip the next line
	std::getline(KGListFile, testline);
	
	CK_SpriteMaps.clear();
	struct CK_SprBox sprinfo;
		
	for(int i = 0; i < CK_MAX_SPRITES; i++){ 
		std::string entireline;
		// Get the line
		std::getline(KGListFile, entireline);
		if(entireline.length() < 40) break;

		sprinfo.clear();

		int spritenumber = std::stoi(entireline.substr(0,4));
		std::string spritebmp = entireline.substr(6,12);
		
		std::string sprboxstr = entireline.substr(19,16);
		std::string sproffstr = entireline.substr(37,8);
		std::stringstream strstream;

		// Get the info from each sub string
		strstream << sprboxstr;
		if(strstream.get()=='('){
			char junk;
			strstream >> sprinfo.left >> junk;
			strstream >> sprinfo.top >> junk;
			strstream >> sprinfo.right >> junk;
			strstream >> sprinfo.bottom;
			if (strstream.fail()) {
				// It failed???
				std::cout << "Failed to read correct values! (box)" << std::endl;
				strstream.clear();
				return 1;
			}
			sprinfo.readbox = true;
		}
		
		strstream.str(sproffstr); // Set to new data
		strstream << sproffstr;
		if(strstream.get()=='['){
			char junk;
			strstream >> sprinfo.offx >> junk;
			strstream >> sprinfo.offy;
			if (strstream.fail()) {
				// It failed???
				std::cout << "Failed to read correct values! (offset)" << std::endl;
				strstream.clear();
				return 1;
			}
			sprinfo.readoff = true;
			
		}else{
			// Default to 0
			sprinfo.offx = sprinfo.offy = 0;
		}

		std::cout << "#" << spritenumber << " : " << spritebmp ;
		std::cout << " box: " << sprinfo.left <<  ", " << sprinfo.top <<  ", " << sprinfo.right << ", " << sprinfo.bottom << " | ";
		std::cout << " off: " << sprinfo.offx  << ", " << sprinfo.offy;
		std::cout << std::endl;
		CK_SpriteMaps.emplace(std::make_pair(spritenumber, sprinfo));
	}
	sprinfo.left = sprinfo.right = sprinfo.top = sprinfo.bottom = sprinfo.offx = sprinfo.offy = 0;
	CK_SpriteMaps.emplace(std::make_pair(9999, sprinfo));

	return 0;
};


unsigned char RGB2PAL(const unsigned char &colorid){
	// fix here
	if(colorid < 5){ return colorid+1; } // Replace purple with dark red
	if(colorid >= 16) { return 0; } // Transparent at >= 16
	return colorid;
};

struct pxls {
	unsigned char b[32];
};

struct Bitmap {
	int width = 0;
	int height= 0;
	
	uint16_t BPP= 0;
	std::vector<unsigned char> pixels;
	std::vector<pxls> blockedImage;
	
	void cleanup(){
		std::vector<unsigned char> org_pixels = pixels;
		pixels.clear();
		std::cout << "Clipping bitmap..." << std::endl;
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width/2; x++){
				pixels.push_back( org_pixels.at((y*width)+x) );
			}
		}
		width /= 2;
	};
	
	void resize(int bwidth, int bheight, unsigned char clearcol = 0x10){
		std::vector<unsigned char> oldpixels = pixels;
		pixels.clear();
		for(int y = 0; y < bheight; y++){
			for(int x = 0; x < bwidth; x++){
				if(x < width && y < height){
					pixels.push_back( oldpixels.at((y*width)+x) );
				}else{
					pixels.push_back( clearcol );
				}
			}
		}
		std::cout << "Resized Bitmap to: " << bwidth << "x" << bheight << std::endl;
		std::cout << "             From: " << width << "x" << height << std::endl;
		width = bwidth;
		height = bheight;
	};
	
	void concat(const struct Bitmap &bmp){
		int oldwidth = width;
		int oldheight = height;
		int nwidth = bmp.width + width;
		int nheight = height;

		if(bmp.height > height) nheight = bmp.height;
		resize(nwidth,nheight);
		std::cout << "Concated Bitmap to: " << width << "x" << height << std::endl;
		
		// Copy the data over
		int bx = oldwidth;
		int by = 0; // nheight - (nheight-oldheight);

		for(int y = 0; y < bmp.height; y++){
			bx = oldwidth;
			for(int x = 0; x < bmp.width; x++){
				unsigned char pixel = bmp.pixels.at((y*bmp.width)+x) ;
				pixels.at((by*nwidth)+bx) =   pixel;
				bx++;
			}
			by++;
		}
	};
	
	void block(){
		// Write the actual data as 8x8 blocks
		for(int y = 0; y < height; y+=8){
			for(int x = 0; x < width; x+=8){
				pxls blockimg;
				for(int i = 0; i < 32; i++){
					blockimg.b[i] = 0;
				}
				for(int by = 0; by < 8; by++){
					for(int bx = 0; bx < 8; bx+=2){
						unsigned char pd = RGB2PAL(pixels[((y+by)*width) + x+bx+1]);
						pd <<= 4;
						pd |=  RGB2PAL(pixels[((y+by)*width) + x+bx]);
						blockimg.b[(by*4)+(bx>>1)] = pd;
					}
				}
				blockedImage.push_back(blockimg);
			}
		}
	};
	
	void fixBox(CK_SprBox &box){
		// Fix the box info
		std::cout << "Reading Box Info" << std::endl;
		box.left = -1;
		box.top = -1;
		box.right = -1;
		box.bottom = -1;
		bool thrownerror = false;
		for(int y = 0; y < height; y++){
			unsigned char prevpixel = pixels.at((y*width)+width/2);
			for(int x = 0; x < width/2; x++){
				unsigned char pixel = pixels.at((y*width)+x+(width/2));
				if(pixel != prevpixel){
					if(pixel != 12){
						if(!thrownerror){
							std::cout << "WARN! Invalid color used!" << std::endl;
							thrownerror = true;
						}
					}
					if(prevpixel == 8){
						if(box.left == -1){
							box.left = x;
						}
					}else{
						if(box.right == -1){
							box.right = x;
						}
					}
				}
				prevpixel = pixel;
			}
		}

		for(int x = 0; x < width/2; x++){
			unsigned char prevpixel = pixels.at(x+(width/2));
			for(int y = 0; y < height; y++){
				unsigned char pixel = pixels.at((y*width)+x+(width/2));
				if(pixel != prevpixel){
					if(pixel != 12){
						if(!thrownerror){
							std::cout << "WARN! Invalid color used!" << std::endl;
							thrownerror = true;
						}
					}
					if(prevpixel == 8){
						if(box.top == -1){
							box.top = y;
						}
					}else{
						if(box.bottom == -1){
							box.bottom = y;
						}
					}
				}
				prevpixel = pixel;
			}
		}
		// If the values are all -1, it's the entire sprite
		if(box.left == -1){
			box.left = 0;
		}
		if(box.right == -1){
			box.right = width/2;
		}
		if(box.top == -1){
			box.top = 0;
		}
		if(box.bottom == -1){
			box.bottom = height;
		}
		

		// Changed to make the width and height subtract 1 ??? why is keen like this?
		box.right -= 1;
		box.bottom -= 1;

		std::cout << "Read box size of: " << box.left << ", " << box.top << ", " << box.right << ", " <<box.bottom << std::endl ;
	};
};



std::vector<unsigned char> BMPHeader;
uint16_t BMPH_BPP;

void LoadBMPHeader(std::string bmpdir){
	
	std::ifstream BMPFile;
	BMPFile.open(bmpdir);
	if(!BMPFile.is_open()){
		std::cerr << "Could not open Bitmap file: " << bmpdir << std::endl;
		return;
	}
	char hash[2];
	BMPFile.read((char*)hash,2);
	if(hash[0] != 'B' || hash[1] != 'M'){
		std::cerr << bmpdir << " is not a valid bitmap file" << std::endl;
		return;
	}
	std::cout << "Loading header from " << bmpdir << std::endl;
	// Get pixels location
	BMPFile.seekg(0xA,std::ios_base::beg);
	uint32_t pixeldata;
	BMPFile.read((char*)&pixeldata,4);
	// Get the BPP
	BMPFile.seekg(0x1C,std::ios_base::beg);
	BMPFile.read((char*)&BMPH_BPP,2);
	
	// Skip to begining
	BMPFile.seekg(0,std::ios_base::beg);
	while(BMPFile.tellg() != pixeldata){
		char byte = 0;
		BMPFile.read(&byte,1);
		BMPHeader.push_back(byte);
	}

	BMPFile.close();
	
	std::cout << "Loaded bitmap header" << std::endl;
	
};

Bitmap LoadBitmap(std::string bmpdir){
	Bitmap bmp;
	
	std::ifstream BMPFile;
	BMPFile.open(bmpdir);
	if(!BMPFile.is_open()){
		std::cerr << "Could not open Bitmap file: " << bmpdir << std::endl;
		return bmp;
	}
	char hash[2];
	BMPFile.read((char*)hash,2);
	if(hash[0] != 'B' || hash[1] != 'M'){
		std::cerr << bmpdir << " is not a valid bitmap file" << std::endl;
		return bmp;
	}
	std::cout << "Loading " << bmpdir << std::endl;
	// Get pixels location
	BMPFile.seekg(0xA,std::ios_base::beg);
	uint32_t pixeldata;
	BMPFile.read((char*)&pixeldata,4);
	// Skip to size info
	BMPFile.seekg(0x12,std::ios_base::beg);

	BMPFile.read((char*)&bmp.width,4);
	BMPFile.read((char*)&bmp.height,4);
	// Get the BPP
	BMPFile.seekg(0x1C,std::ios_base::beg);
	BMPFile.read((char*)&bmp.BPP,2);
	std::cout << "::Bitmap BPP is: " << bmp.BPP << std::endl;
	// Get the palette size
	uint32_t bmpPalSize;
	BMPFile.seekg(0x2E,std::ios_base::beg);
	BMPFile.read((char*)&bmpPalSize,4);
	if(bmpPalSize == 0){
		bmpPalSize = 1<<(bmp.BPP);
	}
	std::cout << "::Palette size:" << bmpPalSize << std::endl;
	// Skip to pixels
	BMPFile.seekg(pixeldata,std::ios_base::beg);
	
	bmp.pixels.resize(bmp.height*bmp.width);
	for(int y = bmp.height-1; y >= 0; y--){
		for(int x = 0; x < bmp.width; x++){
			if(bmp.BPP == 8 || bmp.BPP == 0){
				BMPFile.read((char*)&bmp.pixels[(y*bmp.width)+x],1);
			}
			if(bmp.BPP == 4){
				unsigned char byte;
				BMPFile.read((char*)&byte,1);
				bmp.pixels[(y*bmp.width)+x] = byte>>4;
				bmp.pixels[(y*bmp.width)+x+1] = byte&0xF;
				x++;
			}
		}
	}
	
	BMPFile.close();
	
	std::cout << "::Loaded bitmap is " << bmp.width << "x" << bmp.height << std::endl;
	
	return bmp;
};


void SaveBitmap(const Bitmap &bmp, std::string bmpdir){

	std::vector<unsigned char> NewHeader = BMPHeader;

	// Replace the bitmap size
	uint32_t bmpsize = BMPHeader.size() + bmp.pixels.size();
	if(bmp.BPP==4){
		bmpsize = BMPHeader.size() + (bmp.pixels.size()/2);
	}

	// Bitmap numbers are LE
	NewHeader[2] = (bmpsize)&0xFF;
	NewHeader[3] = (bmpsize>>8)&0xFF;
	NewHeader[4] = (bmpsize>>16)&0xFF;
	NewHeader[5] = (bmpsize>>24)&0xFF;

	// Write the new width & height
	NewHeader[0x12] = (bmp.width)&0xFF;
	NewHeader[0x13] = (bmp.width>>8)&0xFF;
	NewHeader[0x14] = (bmp.width>>16)&0xFF;
	NewHeader[0x15] = (bmp.width>>24)&0xFF;

	NewHeader[0x16] = (bmp.height)&0xFF;
	NewHeader[0x17] = (bmp.height>>8)&0xFF;
	NewHeader[0x18] = (bmp.height>>16)&0xFF;
	NewHeader[0x19] = (bmp.height>>24)&0xFF;
	
	std::ofstream BMPFile;
	BMPFile.open(bmpdir);
	if(!BMPFile.is_open()){
		std::cerr << "Could not open Bitmap file: " << bmpdir << std::endl;
		return;
	}
	BMPFile.write((char*)NewHeader.data(),NewHeader.size());
	std::cout << "Saving " << bmpdir << std::endl;
	std::cout << "Size: " << bmp.width << "x" << bmp.height << std::endl;
	std::cout << "BPP: " << bmp.BPP << std::endl;
	for(int y = bmp.height-1; y >= 0; y--){
		for(int x = 0; x < bmp.width; x++){
			if(bmp.BPP == 8 || bmp.BPP == 0){
				BMPFile.write((char*)&bmp.pixels[(y*bmp.width)+x],1);
			}
			if(bmp.BPP == 4){
				unsigned char byte;
				byte = bmp.pixels[(y*bmp.width)+x]<<4;
				byte |= bmp.pixels[(y*bmp.width)+x+1]&0xF;
				BMPFile.write((char*)&byte,1);
				x++;
			}
		}
	}
	
	BMPFile.close();
	
	std::cout << "::Saved bitmap as " << bmp.width << "x" << bmp.height << std::endl;
};


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


// The different sizes of sprites which are possible
typedef enum GBA_SpriteSizes {
    GBA_SPR_8_8, // 0
    GBA_SPR_16_16, // 1
    GBA_SPR_32_32, // 2
    GBA_SPR_64_64, // 3
    GBA_SPR_16_8, // 4
    GBA_SPR_32_8, // 5
    GBA_SPR_32_16, // 6
    GBA_SPR_64_32, // 7
    GBA_SPR_8_16, // 8
    GBA_SPR_8_32,  // 9
    GBA_SPR_16_32, // 10
    GBA_SPR_32_64 // 11
}GBA_SpriteSizes;

int GBASpr_Dists[12][2] = {
	{8,8}, {16,16}, {32,32}, {64,64}, {16,8}, {32,8}, {32,16}, {64,32},
	{8,16}, {8,32}, {16,32}, {32,64},
};

int sprchkopt[12] = {
	3,7,11,2,6,5,10,1,4,9,8,0
};

std::string GBASizes[] = {
	"8x8","16x16","32x32","64x64","16x8","32x8","32x16","64x32",
	"8x16","8x32","16x32","32x64"};


struct GBASprInfo {
	int spr_rel_x;
	int spr_rel_y;
	int spr_gfx_x;
	int spr_gfx_y;
	GBA_SpriteSizes spr_type;
};

struct SpriteInfo {
	std::string name = "";
	std::vector<int> bitmapids;
	std::vector<Bitmap> bitmapgfx;
	int maxwidth = 0;
	int maxheight = 0;
	Bitmap newbitmap;
	std::vector<GBASprInfo> GBAInfo;
	std::vector<CK_SprBox> sprboxes;
	std::vector<int> sproffy;
};


std::vector<SpriteInfo> SpriteList;



void ExportSprite(SpriteInfo &sprinfo, std::string exportdir){
	Bitmap &bmp = sprinfo.newbitmap;
	
	std::ofstream CFile;
	CFile.open(exportdir);
	if(!CFile.is_open()){
		std::cout << "Failed to open file: " << exportdir << std::endl;
		return;
	}
	std::cout << "Exporting to:" << exportdir << std::endl;

	CFile << "// Sprite sheet " << sprinfo.name << " exported by ckspr2sheet by John314\n"; 
	CFile << "// \n ";
	CFile << "const unsigned int " << sprinfo.name << "_width = " <<(int)bmp.width << ";\n";
	CFile << "const unsigned int " << sprinfo.name << "_height = " <<(int)bmp.height << ";\n";
	CFile << "const unsigned int " << sprinfo.name << "_size = " <<(int)(bmp.blockedImage.size()*32) << ";\n";
	CFile << "const unsigned char " << sprinfo.name << "_data[] = {\n";
	// Write the blocked data
	int count = 0;
	for(int i = 0; i < bmp.blockedImage.size(); i++){
		for(int e = 0; e < 32; e++){
			if((count%64)==0){
				CFile << "\n\t";
			}
			count++;
			CFile << "0x" << std::hex << (int)bmp.blockedImage.at(i).b[e] << ", ";
		}
		CFile << "\n\t";
	}
	
	CFile << "};\n\n";
	// Write all the sprite hitbox / offset info
	CFile << "// Stores data as { boxl,boxt,boxr,boxb,offx,offy }\n";
	CFile << "const signed short " << sprinfo.name << "_boxinfo[] = {\n";
	// Write the info
	int imgcount = 0;
	for(CK_SprBox &info : sprinfo.sprboxes){
		if(!info.readbox){
			// Fix the hitbox now
			info.left += info.offx;
			info.right += info.offx;
			info.top += info.offy;
			info.bottom += info.offy;
		}
		CFile << "\t" << std::dec << info.left << ", " << info.top << ", " << info.right << ", " << info.bottom << ", " << info.offx << ", " << info.offy << ", //" << 	sprinfo.bitmapids.at(imgcount) << "\n";
		imgcount++;
	}
		
	CFile << "};\n";/*
	// Write all the gba sprite offsets
	CFile << "\nconst signed short " << sprinfo.name << "_gbainfo[] = {";
	// Write the info
	int blockcount = 0;
	for(int &offy : sprinfo.sproffy){
		if((blockcount%32)==0){
			CFile << "\n\t";
		}
		CFile << std::dec << (sprinfo.maxheight-offy) << ", " ;
		blockcount++;
	}
		
	CFile << "};\n";*/
	CFile.close();
};



void parseSpriteString(std::string str){
	if(!str.length()) return;
	std::string name;
	std::string picstring;
	// Convert to stringstream
	std::stringstream ss(str);
	ss >> name >> picstring;
	std::string cleanline;
	for(char c : picstring){
		if(c == ',' || c == '-' || isdigit(c)){
			cleanline += c;
		}else{
			break; // Bad string data now
		}
	}
	picstring = cleanline;
	// Inform the user
	std::cout << "Adding sprite: " << name << std::endl;
	std::cout << "--Collecting sprite #:" << picstring << std::endl;
	// Actually push to a useable variable
	SpriteInfo sprinfo;
	sprinfo.name = name;

	char *c_picstr = (char*)malloc(picstring.length()+2);
	memset(c_picstr, 0, picstring.length()+2);
	memcpy(c_picstr, picstring.c_str(), picstring.length());
	char *c_picnext;

	std::cout << "LINE:" << c_picstr << std::endl;

	long bmpval = strtol(c_picstr, &c_picnext, 10);
	while(bmpval){
		if((*c_picnext) == '-'){
			c_picnext++; // Skip the dash
			int bmpstart = bmpval;
			int bmpend = strtol(c_picnext, &c_picnext, 10);
			
			std::cout << "-- BMPS: " << bmpstart << " to " << bmpend << std::endl;
			for(int bmpid = bmpstart; bmpid <= bmpend; bmpid++){
				sprinfo.bitmapids.push_back(bmpid);
			}		
			if((*c_picnext) == ','){
				c_picnext++;
			}
		}else if((*c_picnext) == ','){
			std::cout << "-- BMP: " << bmpval << std::endl;
			sprinfo.bitmapids.push_back(bmpval);
			c_picnext++;
		}else {
			std::cout << "-- BMP: " << bmpval << std::endl;
			sprinfo.bitmapids.push_back(bmpval);
			break;
		}
		bmpval = strtol(c_picnext, &c_picnext, 10);
	}
	
	SpriteList.push_back(sprinfo);
};



int main(int argc, char *args[]){
	
	if(LoadSpriteBoxes("SPRITES")) return 0;
		
	std::ifstream spritelist;
	spritelist.open("SPRITES.txt");
	if(!spritelist.is_open()){
		std::cout << "Failed to open SPRITES.txt" << std::endl;
		return 1;
	}
	std::string sline;
	while (getline (spritelist, sline)){
		parseSpriteString(sline);
	}
	
	LoadBMPHeader("TEMPLATE.BMP");

	std::ofstream SprDefFile, SprDefFileH;
	SprDefFile.open("EXPORT/CK_SpriteDefs.c");
	SprDefFileH.open("EXPORT/CK_SpriteDefs.h");
	if(!SprDefFile.is_open()){
		std::cout << "Failed to open EXPORT/CK_SpriteDefs.c" << std::endl;
		return 0;
	}
	if(!SprDefFileH.is_open()){
		std::cout << "Failed to open EXPORT/CK_SpriteDefs.h" << std::endl;
		return 0;
	}
	
	SprDefFile << "// Sprite GBA sheet generated by ckspr2sheet bu John314\n";
	SprDefFile << "//\n";

	SprDefFileH << "// Sprite GBA sheet generated by ckspr2sheet bu John314\n";
	SprDefFileH << "//\n";

	std::ofstream HITBFile;

	HITBFile.open("CLEAN_LIST.TXT");
	
	// Load all the graphics
	for(SpriteInfo &info : SpriteList){
		info.maxwidth = 0;
		info.maxheight = 0;
		for(int & bmpval : info.bitmapids){
			std::stringstream ss;
			ss << "SPRITES/CK_SPR" << std::dec << std::setw(4) << std::setfill('0') << bmpval << ".BMP";
			info.bitmapgfx.push_back(LoadBitmap(ss.str()));
			// Read any hitbox data if needed
			auto &data = CK_SpriteMaps.at(bmpval); // Will throw exception if somthing bad happens!
			if(!data.readbox){
				info.bitmapgfx.back().fixBox(data);
			}
			// Add to the list
			info.sprboxes.push_back(data);
			info.sproffy.push_back(info.bitmapgfx.back().height);

			// Optimize the bitmap
			info.bitmapgfx.back().cleanup();
			// Finx the max size
			if(info.maxwidth < info.bitmapgfx.back().width)
				info.maxwidth = info.bitmapgfx.back().width;
			if(info.maxheight < info.bitmapgfx.back().height)
				info.maxheight = info.bitmapgfx.back().height;
			HITBFile << bmpval << " : (";
			HITBFile << data.left << "," << data.top << ",";
			HITBFile << data.right << "," << data.bottom << ") ";
			HITBFile << "[" << data.offx << "," << data.offy << "] \n";
		}

		std::cout << "Largest size:" << info.maxwidth << "x" << info.maxheight << std::endl;
		// Make sure it's divisable by 8
		if(info.maxheight < 8) { 
			info.maxheight = 8;
		}
		if(info.maxwidth < 8) { 
			info.maxwidth = 8;
		}
		int widthmultiple = info.maxwidth/8;
		int heightmultiple = info.maxheight/8;

		if((heightmultiple * 8) < info.maxheight) heightmultiple ++;
		if((widthmultiple * 8) < info.maxwidth) widthmultiple ++;

		info.maxheight = heightmultiple * 8;
		info.maxwidth = widthmultiple * 8;
		std::cout << "Fixed size:" << info.maxwidth << "x" << info.maxheight << std::endl;
		// Create a new concatinated bitmaps
		for(Bitmap &bmp : info.bitmapgfx){
			bmp.resize(info.maxwidth, info.maxheight);
			info.newbitmap.concat(bmp);// = bmp;//.concat(bmp);
		}
		SaveBitmap(info.newbitmap, "EXPORT/BMP/"+info.name+".bmp");

		Bitmap hitbmp = info.newbitmap;
		const int bmplen = info.maxwidth;
		for(int bmpval = 0; bmpval < info.bitmapids.size(); bmpval++){
			auto &bxdata = CK_SpriteMaps.at(info.bitmapids.at(bmpval));
			for(int i = bxdata.left; i < bxdata.right && i < hitbmp.width; i++){
				int y = bxdata.top;
				if(y >= 0 && y < hitbmp.height && i >= 0 && i < bmplen)
					hitbmp.pixels.at(( y*hitbmp.width)+i + (bmpval*bmplen)) = 0x1;
				y = bxdata.bottom;
				if(y >= 0 && y < hitbmp.height && i >= 0 && i < bmplen)
					hitbmp.pixels.at(( y*hitbmp.width)+i + (bmpval*bmplen)) = 0x2;
			}
			for(int e = bxdata.top; e < bxdata.bottom && e < hitbmp.height; e++){
				int x = bxdata.left + (bmpval*bmplen);
				if(e >= 0 && e < hitbmp.height && x >= 0 && x < hitbmp.width)
					hitbmp.pixels.at((e*hitbmp.width)+x) = 0x3;
				x = bxdata.right + (bmpval*bmplen);
				if(e >= 0 && e < hitbmp.height && x >= 0 && x < hitbmp.width)
					hitbmp.pixels.at((e*hitbmp.width)+x) = 0x4;
			}
			if(bxdata.readbox){
				hitbmp.pixels.at(bmpval*bmplen) = 0x0;
			}
		}
		SaveBitmap(hitbmp, "EXPORT/HIT/"+info.name+".bmp");
	
		// Determine the sprite info
		int spr_width = info.maxwidth;
		int spr_height = info.maxheight;
		int spr_offset_x = 0;
		int spr_offset_y = 0;

		std::cout << "Finding optimum sprite layout... " << std::endl;
		std::cout << "Full size:" << spr_width << "x" << spr_height << std::endl;
		
		int sprBlocksX = spr_width/8;
		int sprBlocksY = spr_height/8;
		int sprcheck[sprBlocksY][sprBlocksX];
		int blocksfilled = 0;
		
		memset(&sprcheck, 0, sizeof(sprcheck));
		while(blocksfilled != (sprBlocksY*sprBlocksX) || spr_offset_y > info.maxheight){
			int spri  = 0;
			for(spri = 0; spri < 12; spri ++){
				int sprchk = sprchkopt[spri];
				bool isGoodSize = true;
				for(int ry = 0; ry < GBASpr_Dists[sprchk][1]/8; ry++){
					for(int rx = 0; rx < GBASpr_Dists[sprchk][0]/8; rx++){
						int cx = rx+(spr_offset_x/8);
						int cy = ry+(spr_offset_y/8);
						if(sprcheck[cy][cx] || cx >= (sprBlocksX) || cy >= (sprBlocksY)){
							// Bad size
							isGoodSize = false;
							goto endloop;
						}
					}
				}
				endloop:;
				if(!isGoodSize) continue;
				
				for(int ry = 0; ry < GBASpr_Dists[sprchk][1]/8; ry++){
					for(int rx = 0; rx < GBASpr_Dists[sprchk][0]/8; rx++){
						sprcheck[ry+(spr_offset_y/8)][rx+(spr_offset_x/8)] = spri+1;
						blocksfilled++;
					}
				}
				
				GBASprInfo tmpinfo;
				tmpinfo.spr_rel_x = spr_offset_x;
				tmpinfo.spr_rel_y = spr_offset_y;
				tmpinfo.spr_gfx_x = tmpinfo.spr_gfx_y = 0;
				tmpinfo.spr_type = (GBA_SpriteSizes)sprchk;
				info.GBAInfo.push_back(tmpinfo);
				/*
				std::cout << "(" << std::hex << spri << ")================================" << std::endl;
				std::cout << "Sprite size:" << std::dec << GBASizes[sprchk] << std::endl;
				std::cout << "Move: " << GBASpr_Dists[sprchk][0] << "," << GBASpr_Dists[sprchk][1] << std::endl;
				std::cout << "Pos: " << spr_offset_x << "," << spr_offset_y << std::endl;
				std::cout << "==================================" << std::endl;
				for(int y = 0; y < sprBlocksY; y++){
					for(int x = 0; x < sprBlocksX; x++){
						std::cout << std::hex << sprcheck[y][x];
					}
					std::cout << std::endl;
				}*/

				spr_offset_x += GBASpr_Dists[sprchk][0];
				break;
			}
			if(spri==12){
				spr_offset_x += 8;
			}
			if(spr_offset_x >= info.maxwidth){
				spr_offset_x = 0;
				spr_offset_y += 8;
			}
		}
		std::cout << "==================================" << std::endl;
		for(int y = 0; y < sprBlocksY; y++){
			for(int x = 0; x < sprBlocksX; x++){
				std::cout << ((sprcheck[y][x])?'X':'.');
			}
			std::cout << std::endl;
		}
		// Blockify the image
		info.newbitmap.block();
				
		// Print the info
		// And write to a file
		SprDefFileH << "extern const unsigned int " << info.name << "_width;\n";
		SprDefFileH << "extern const unsigned int " << info.name << "_height;\n";
		SprDefFileH << "extern const unsigned int " << info.name << "_size;\n";
		SprDefFileH << "extern const unsigned char " << info.name << "_data[];\n";
		SprDefFileH << "extern const signed short " << info.name << "_boxinfo[];\n";
//		SprDefFileH << "extern const signed short " << info.name << "_gbainfo[];\n";

		SprDefFileH << "extern const unsigned int " << info.name << "_SprSize;\n";
		SprDefFileH << "extern const unsigned int " << info.name << "_SprCount;\n";
		SprDefFileH << "extern const unsigned int " << info.name << "_SprData[];\n";


		SprDefFile << "const unsigned int " << info.name << "_SprSize = " << (int)(info.maxwidth*info.maxheight) << ";\n";
		SprDefFile << "const unsigned int " << info.name << "_SprCount = " << info.GBAInfo.size() << ";\n";
		SprDefFile << "const unsigned int " << info.name << "_SprData[] = {\n";

		std::vector<pxls> newblocks;
		int blockoffset = 0;
		for(GBASprInfo &spr : info.GBAInfo){

			spr.spr_gfx_x = blockoffset;
			// Sort the blocks
			int BlocksPerLayer = (info.newbitmap.width/8);
			int SpritesPerLayer = (info.newbitmap.width/spr_width);
			int SprBlockX = (spr_width/8);
			int SprBlockY = (spr_height/8);
			
			std::cout << "Final Size: " << info.newbitmap.width << "x" << info.newbitmap.height << std::endl;
			std::cout << "BlocksPerLayer:" << BlocksPerLayer << std::endl;
			std::cout << "SpritesPerLayer:" << SpritesPerLayer << std::endl;
			std::cout << "BlocksPerSprite:" << SprBlockX << " x " << SprBlockY << std::endl;

			for(int blockc = 0; blockc < SpritesPerLayer; blockc++){
				int frameheight = GBASpr_Dists[spr.spr_type][1]/8;
				int framewidth = GBASpr_Dists[spr.spr_type][0]/8;
				for(int ry = 0; ry < frameheight; ry++){
					for(int rx = 0; rx < framewidth; rx++){
						int cx = rx+(spr.spr_rel_x/8);
						int cy = ry+(spr.spr_rel_y/8);

						int blockid = (cy*BlocksPerLayer)+cx + (blockc*SprBlockX);
						newblocks.push_back(info.newbitmap.blockedImage.at(blockid));
						blockoffset++;
					}
				}
			}
//			blockoffset += (GBASpr_Dists[spr.spr_type][1]/8)*(GBASpr_Dists[spr.spr_type][0]/8);

			SprDefFile << "\t" << (int)spr.spr_type << ", " << spr.spr_rel_x  << ", " << spr.spr_rel_y << ", " << spr.spr_gfx_x << ", // " << std::endl;
			
			std::cout << "Sprite Size: " << GBASizes[spr.spr_type] << std::endl;
			std::cout << "--X: " << spr.spr_rel_x << std::endl;
			std::cout << "--Y: " << spr.spr_rel_y << std::endl;
		}
		SprDefFile << "};\n";
		info.newbitmap.blockedImage = newblocks;

		// Export the image
		ExportSprite(info, "EXPORT/C/"+info.name+".c");
	}
	
	// Make a list of pointers
	SprDefFileH << "\n\nconst unsigned int *CK_SpritePtrs[] = {\n";
	for(SpriteInfo &info : SpriteList){
		SprDefFileH << "\t(unsigned int*)" << info.name << "_SprData, &" << info.name << "_SprCount, (unsigned int*)" << info.name << "_data, &" << info.name << "_SprSize" << ", (unsigned int*)" << info.name << "_boxinfo, \n";
	}
	SprDefFileH << "};\n";
/*
	// Make a list of pointers
	SprDefFileH << "\n\nconst signed short *CK_SpriteOffsets[] = {";
	int sprcont = 0;
	for(SpriteInfo &info : SpriteList){
		if((sprcont%8)==0){
			SprDefFileH << "\n\t";
		}
		sprcont++;
		SprDefFileH << "&" << info.name << "_gbainfo, ";
	}
	SprDefFileH << "};\n";
	
	SprDefFile.close();
	SprDefFileH.close();*/
	
	std::ofstream SpectorH;
	SpectorH.open("EXPORT/CKx_Spectors.h");
	if(!SpectorH.is_open()){
		std::cout << "Failed to open EXPORT/CKx_Spectors.h" << std::endl;
		return 0;
	}
	// Make a list of pointers
	SpectorH << "// Generated by ckspr2sheet\n\n#ifndef __CK4_SPECTORS__\n#define __CK4_SPECTORS__\n\ntypedef enum CK_SpriteType{\n";
	for(SpriteInfo &info : SpriteList){
		std::string capname = "";
		for(char &c : info.name){
			capname += toupper(c);
		}
		SpectorH << "\t" << capname << ",\n";
	}
	SpectorH << "}CK_SpriteType;\n#endif\n";	
//	SpectorH << "\tCKS_EOL // End of list\n}CK_SpriteType;\n#endif\n";	
	SpectorH.close();
	
	return 0;
};

