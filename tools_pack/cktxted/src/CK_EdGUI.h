//
// Commander Keen Text Editor
//
// Editor for laying out help / story texts
//
// 06/21/2023
//

#pragma once

#define MAX_LINES 18 // 16 for help, 18 for story
#define MAX_LINE 28

typedef struct page_img_t {
	int x;
	int y;
	int blockx;
	int blocky;
	int imgid;
    int time;
}page_img;


typedef struct page_block_t {
	int x;
	int y;
    int w;
    int h;
	int blockx;
	int blocky;
	int color;
    int time; // ???
}page_block;

typedef struct t_char_t {
	char ch;
	char color;
}t_char;

typedef struct page_t{
	t_char lines[MAX_LINES][MAX_LINE];
	std::vector<page_img> imgs;
	std::vector<page_block> blocks;
	bool timedpage;
	void clear(){
		timedpage = false;
		imgs.clear();
		blocks.clear();
		for(int i = 0; i < MAX_LINES; i++){
			for(int e = 0; e < MAX_LINE; e++){
				lines[i][e].ch = 0;
				lines[i][e].color = 0;
			}
		}
	}
}ck_page;


const unsigned int MAX_IMAGES = 128;

void EdGUI_HandleInput();
void EdGUI_Draw();
void EdDrawChar(int x, int y, char c, RR_Pixel pixel);
void EdWriteString(std::string str, int x, int y, RR_Pixel pixel);

