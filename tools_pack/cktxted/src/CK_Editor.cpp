//
// Commander Keen Text Editor
//
// Editor for laying out help / story texts
//
// 06/12/2023
//

#include "CK_Editor.h"
#include "CK_EdGUI.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

unsigned int MaxLines = MAX_LINES;
std::vector<ck_page> m_Pages;

// https://moddingwiki.shikadi.net/wiki/EGAGraph_Format#Game%20texts
/*
^P		First command in every file. Defines a page start
^E		Ends the file
^Cx		Change font color to $x until next page or ^C command
^Gx,y,z	Display (unmasked) picture chunk z at location x,y (in pixels)
^Tx,y,z,t	Display picture chunk z at x,y for z clicks of time
^Bx,y,z,t,b	Fill a width-by-height-pixel rectangle at pixel location x,y (for z clicks of time?) with color $x
^Lx,y		Start text alignment at pixel location x,y.
*/

int parseInt(std::ifstream &file){
	int result = 0;
	while(!file.eof()){
		char c;
		file.get(c);
		if(c >= '0' && c <='9'){
			// We good
			result *= 10;
			result += c-'0';
		}else{
			// Fix the charater buffer
			file.putback(c);
			return result;
		}
	}
	return result;
};

char hexStr[] = "0123456789ABCDEF";

bool CK_LoadText(std::string filename){
	std::ifstream file(filename);
	if(file.is_open()){
		ck_page n_page;
		page_img cur_img;
		page_block cur_block;
		char cur_col = 14; // Default is 15???
		int txt_x = 0;
		int txt_y = 0;
		n_page.clear();
		std::cout << "Loading Text " << filename << std::endl;
		while(!file.eof()){
			char c;
			file.get(c);
			if(c=='^'){
				file.get(c);
				c = toupper(c); // Hmmm
				// Check all operator values
				switch(c){
					case 'P':
						// New Page
						m_Pages.push_back(n_page);
						n_page.clear();
						txt_x = 0;
						txt_y = -1; // Remove the top new line
						for(int ll = 0; ll < MAX_LINES; ll++){
							for(int llx = 0; llx < MAX_LINE; llx++){
								n_page.lines[ll][llx].color = cur_col;
							}
						}
					break;
					case 'E':
						// End of text
						m_Pages.push_back(n_page);
						n_page.clear();
						file.close();
						m_Pages.erase(m_Pages.begin()); // Remove the first page cause it's blank
						std::cout << "Done Loading Text" << std::endl;
						return true;
					break;
					case 'C':
						file.get(c);
						c = toupper(c); // Hmmm
						for(int i = 0; i < 16; i++){
							if(hexStr[i]==c){
								cur_col = i;
								break;
							}
						}
						// Fill the page with that color
						for(int llx = txt_x; llx < MAX_LINE; llx++){
							n_page.lines[txt_y][llx].color = cur_col;
						}
						for(int ll = txt_y+1; ll < MAX_LINES; ll++){
							for(int llx = 0; llx < MAX_LINE; llx++){
								n_page.lines[ll][llx].color = cur_col;
							}
						}
					break;
					case 'G':
						// Some reason the Wiki says x,y but it's y,x
						cur_img.y = parseInt(file); file.get(c);
						cur_img.x = parseInt(file); file.get(c);
						cur_img.imgid = parseInt(file); file.get(c); file.get(c);
						cur_img.blockx = cur_img.x>>3;
						cur_img.blocky = cur_img.y>>3;
						cur_img.time = 0;
						// Add an image
						n_page.imgs.push_back(cur_img);
					break;
					case 'T':
						// Get the info
						// Some reason the Wiki says x,y but it's y,x
						cur_img.y = parseInt(file); file.get(c);
						cur_img.x = parseInt(file); file.get(c);
						cur_img.imgid = parseInt(file); file.get(c);
						cur_img.time = parseInt(file);
						cur_img.blockx = cur_img.x>>3;
						cur_img.blocky = cur_img.y>>3;
						// Add an image
						n_page.imgs.push_back(cur_img);
					break;
					case 'B':
						// Some reason the Wiki says x,y but it's y,x
						cur_block.y = parseInt(file); file.get(c);
						cur_block.x = parseInt(file); file.get(c);
						cur_block.time = parseInt(file); file.get(c);
						cur_block.w = parseInt(file)>>3; file.get(c);
						cur_block.h = parseInt(file)>>3;
//						cur_block.color = parseInt(file); 

						cur_block.blockx = cur_block.x>>3;
						cur_block.blocky = cur_block.y>>3;
						// Add a block
						n_page.blocks.push_back(cur_block);
					break;

					case 'L':
						txt_x = parseInt(file)>>3; file.get(c);
						txt_y = parseInt(file)>>3;
						break;
				}
			}else{
				// Move the last word to the new line
				t_char lastword[MAX_LINE] = {0};
				int i, t_wordlen = 0, linenew = 0;
				auto grabLine = [&]() {
					// If a word is longer than the screen uh....
					for(i = MAX_LINE-1; i >= 0; i--){
						if(n_page.lines[txt_y][i].ch == ' ') break;
						lastword[t_wordlen++] = n_page.lines[txt_y][i];
						n_page.lines[txt_y][i] = t_char('\0',0);
					}
				};
				if(c=='\t'){ c = ' '; } // Hmmm
				if(c==0x0D){ file.get(c); }
				if(c==0x0A){
					txt_x = -1; // Make sure the line starts at the start
					txt_y += 1;
					linenew = 1;
				}else{
					txt_x += 1;
					if(txt_x >= MAX_LINE){
						grabLine();
						txt_x = 0;
						txt_y += 1;
						linenew = 1;
					}
				}

				if(txt_y >= MAX_LINES || txt_y >= MaxLines){
					// Spawn new page and continue!
					m_Pages.push_back(n_page);
					n_page.clear();
					txt_x = 0;
					txt_y = 0;
					for(int ll = 0; ll < MAX_LINES; ll++){
						for(int llx = 0; llx < MAX_LINE; llx++){
							n_page.lines[ll][llx].color = cur_col;
						}
					}
				}
				// Plop that bad boy here instead
				if(linenew){
					for(i = t_wordlen-1; i >= 0; i--){
						n_page.lines[txt_y][txt_x] = lastword[i];
						txt_x++;
					}
				}
//				t_char wch;
//				wch.ch = c;
//				wch.color = cur_col;
				n_page.lines[txt_y][txt_x].ch = c;
			}
		}
		file.close();
		std::cout << "No End Page Marker!" << std::endl;
		return false; // Never reached an ^E marker
	}
	return false;
};

bool SavePages(){
	std::ofstream savefile;
	savefile.open("savefile.cktxt");
	if(savefile.is_open()){
		std::cout << "Saving file..." << std::endl;	
		// Write the number of pages
		int data_i = m_Pages.size();
		savefile.write((char*)&data_i,sizeof(data_i));
		// Write all pages
		for(ck_page &p : m_Pages){
			bool data_b = p.timedpage;
			savefile.write((char*)&data_b,sizeof(data_b));
			for(int y = 0; y < MAX_LINES; y++){
				for(int x = 0; x < MAX_LINE; x++){
					t_char chencoded = p.lines[y][x];
					savefile.write((char*)&chencoded,sizeof(chencoded));
				}
			}
			data_i = p.imgs.size();
			savefile.write((char*)&data_i,sizeof(data_i));
			for(page_img &img : p.imgs){
				savefile.write((char*)&img,sizeof(img));
			}
			data_i = p.blocks.size();
			savefile.write((char*)&data_i,sizeof(data_i));
			for(page_block &block : p.blocks){
				savefile.write((char*)&block,sizeof(block));
			}
		}

	}else{
		std::cout << "FAILED TO SAVE!!!" << std::endl;
		return false;
	}
	savefile.close();
	return true;
};

bool LoadPages(std::string filename){
	std::ifstream savefile;
	savefile.open(filename);
	if(savefile.is_open()){
		std::cout << "Loading file..." << std::endl;	
		// Read the number of pages
		int pagecnt = 0;
		savefile.read((char*)&pagecnt,sizeof(int));
		// Read all pages
		ck_page p;
		for(int i = 0; i < pagecnt; i++){
			savefile.read((char*)&p.timedpage,sizeof(p.timedpage));
			for(int y = 0; y < MAX_LINES; y++){
				for(int x = 0; x < MAX_LINE; x++){
					savefile.read((char*)&p.lines[y][x],sizeof(t_char));
				}
			}
			// Read the number of imgs
			int imgcnt = 0;
			page_img timg;
			savefile.read((char*)&imgcnt,sizeof(int));
			for(int e = 0; e < imgcnt; e++){
				savefile.read((char*)&timg,sizeof(timg));
				p.imgs.push_back(timg);
			}
			// Read the number of blocks
			page_block tblock;
			savefile.read((char*)&imgcnt,sizeof(int));
			for(int e = 0; e < imgcnt; e++){
				savefile.read((char*)&tblock,sizeof(tblock));
				p.blocks.push_back(tblock);
			}
			m_Pages.push_back(p);
			p.clear();
		}
	}else{
		std::cout << "FAILED TO LOAD!!!" << std::endl;
		return false;
	}
	savefile.close();
	return true;
};

bool ExportPages(){
	std::ofstream GBAFile;
	std::string filename = "EXPORT";
	GBAFile.open(filename+".c");
	if(GBAFile.is_open()){
		GBAFile << "// Text generated by cktxted by John314\n";
		GBAFile << "// \n";
		GBAFile << "const unsigned int " << filename << "_pages = " << (int)m_Pages.size() << ";\n";
		GBAFile << "const unsigned char " << filename << "_text[] = {\n";
		// Write all the data
		for(ck_page &page : m_Pages){
			for(int i = 0; i < MAX_LINES; i++){
				GBAFile << "\t";
				for(int e = 0; e < MAX_LINE; e++){
					GBAFile << "0x" << std::hex << (int)page.lines[i][e].ch;
					GBAFile << ", ";
				}
				GBAFile << "\n";
			}
			GBAFile << "\n";
		}
		GBAFile << "};\n\n//////////////////////////////////////////////////////////////////////////////////////////////\n";
		GBAFile << "const unsigned int " << filename << "_color[] = {\n";
		// Write all the data
		for(ck_page &page : m_Pages){
			for(int i = 0; i < MAX_LINES; i++){
				GBAFile << "\t";
				for(int e = 0; e < MAX_LINE; e++){
					GBAFile << "0x" << std::hex ;
					for(int ci = 0; ci < 8; ci++){
						GBAFile << (int)page.lines[i][e].color;
					}
					GBAFile << ", ";
				}
				GBAFile << "\n";
			}
			GBAFile << "\n";
		}
		GBAFile << "};\n\n//////////////////////////////////////////////////////////////////////////////////////////////\n";
		GBAFile << "const unsigned int " << filename << "_graphics[] = {\n";
		// Write all the data
		for(ck_page &page : m_Pages){
			GBAFile << "0x" << std::hex << (int)page.imgs.size() << ", \n";
			for(page_img &img : page.imgs){
				GBAFile << "\t";
				GBAFile << "0x" << std::hex << (int)img.blockx << ", ";
				GBAFile << "0x" << std::hex << (int)img.blocky << ", ";
				GBAFile << "0x" << std::hex << (int)img.imgid << ", ";
				GBAFile << "0x" << std::hex << (int)img.time << ", ";
				GBAFile << "\n";
			}
		}
		GBAFile << "};\n";
	}else{
		std::cout << "FAILED TO SAVE!!!" << std::endl;
		return false;
	}
	GBAFile.close();
	return true;
};

extern RR_Image m_BitmapImages[MAX_IMAGES];
extern RR_Image m_BorderBitmap;

RR_Window local_window;

int main(int argc, char *args[]){
	// Where the graphics are stored
	std::string graphicsFolder = "";
	std::string borderFile = "";
	std::string textFile = "";
	// What the bitmap filename starts with
	std::string CK_IMG_HEAD = "4BMP";
	unsigned int BMP_START = 6;
	unsigned int BMP_END = 87;

	for(int ac = 0; ac < argc; ac ++){
		std::string argstr = args[ac];
		if(argstr.length() > 8 && argstr.substr(0,8).compare("-bmpdir=")==0){
			graphicsFolder = argstr.substr(8,argstr.length());
		}
		if(argstr.length() > 9 && argstr.substr(0,9).compare("-bmphead=")==0){
			CK_IMG_HEAD = argstr.substr(9,argstr.length());
		}
		if(argstr.length() > 10 && argstr.substr(0,10).compare("-bmpstart=")==0){
			std::string intval = argstr.substr(10,argstr.length());
			BMP_START = stoi(intval);
		}
		if(argstr.length() > 8 && argstr.substr(0,8).compare("-bmpend=")==0){
			std::string intval = argstr.substr(8,argstr.length());
			BMP_END = stoi(intval);
		}
		if(argstr.length() > 9 && argstr.substr(0,9).compare("-borderf=")==0){
			borderFile = argstr.substr(9,argstr.length());
		}
		if(argstr.length() > 8 && argstr.substr(0,8).compare("-height=")==0){
			std::string intval = argstr.substr(8,argstr.length());
			MaxLines = stoi(intval);
		}
		if(argstr.length() > 6 && argstr.substr(0,6).compare("-text=")==0){
			textFile = argstr.substr(6,argstr.length());
		}
	}

    // Setup the library
    RR_InitLibrary();

    // Create a window
	local_window = RR_CreateWindow("CK Text Editor", 480, 240, 3, 3);
	RR_SetAspectRatio(local_window, true);

	if(graphicsFolder.length() == 0){
		// Ask where the graphics are
		char *folderpath = tinyfd_selectFolderDialog("Open Graphics Folder", ".");
		if(folderpath == NULL){
			// No path specified
			std::cerr << "Error opening graphics folder!" << std::endl;
			RR_DestroyLibrary();
			return 1;
		}
		graphicsFolder = std::string(folderpath);
	}

	std::cout << "Opened graphics folder: " << graphicsFolder << std::endl;

	if(borderFile.length()){
		m_BorderBitmap = RR_LoadImage(local_window, borderFile);
	}else{
		// Ask what border to use
		const char *filters[4] = {"*.png", "*.PNG", "*.bmp", "*.BMP"};
		char *borderfilename = tinyfd_openFileDialog("Open Border Bitmap", ".", 4, filters,"image files",0);
		if(borderfilename == NULL){
			// No path specified
			std::cerr << "Error opening border graphic!" << std::endl;
			RR_DestroyLibrary();
			return 1;
		}
		m_BorderBitmap = RR_LoadImage(local_window, borderfilename);
	}

	const char *filters2[3] = {"*.txt", "*.TXT", "*.cktxt"};
	char *textfilename;
	std::string cmpstr;
	bool faledLoad = false;
	if(!textFile.length()){
		// Ask what text to load
		textfilename = tinyfd_openFileDialog("Open Text File", ".", 3, filters2,"CK text files",0);
		if(textfilename!=NULL){
			cmpstr = textfilename;
		}else{
			faledLoad = true;
		}
	}else{
		cmpstr = textFile;
	}
	if(cmpstr.length() > 6 && cmpstr.substr(cmpstr.length()-6,cmpstr.length()).compare(".cktxt")==0){
		faledLoad = !LoadPages(cmpstr);
	}else{
		faledLoad = !CK_LoadText(cmpstr);
	}
	// Load the text file into a useable format
	if(faledLoad){
		// No path specified
		std::cerr << "Error opening text file! " << cmpstr << std::endl;
		RR_DestroyLibrary();
		return 1;
	}

	// Load all the bitmaps
	for(unsigned int i = BMP_START; i <= BMP_END && i < MAX_IMAGES; i++){
		std::string NumberHash;
		std::stringstream ss;
		ss << std::setw(4) << std::setfill('0') << (i);
		ss >> NumberHash;
		m_BitmapImages[i] = RR_LoadImage(local_window, graphicsFolder+"/"+CK_IMG_HEAD+NumberHash+".BMP");
	}

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        if(RR_CheckWindowClosed(local_window)){ break; }

		EdGUI_HandleInput();

		EdGUI_Draw();
    }

    // Shutdown the library
    RR_DestroyLibrary();
	return 0;
};

