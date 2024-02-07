//
// Commander Keen Text Editor
//
// Editor for laying out help / story texts
//
// 06/21/2023
//

#include "CK_Editor.h"
#include "CK_EdGUI.h"

RR_Color EGA_Colors[16] = {
    RR_RGBA(0,0,0),
    RR_RGBA(0,0,0xAA),
    RR_RGBA(0,0xAA,0),
    RR_RGBA(0,0xAA,0xAA),
    RR_RGBA(0xAA,0,0),
    RR_RGBA(0xAA,0,0xAA),
    RR_RGBA(0xAA,0x55,0),
    RR_RGBA(0xAA,0xAA,0xAA),
    RR_RGBA(0x55,0x55,0x55),
    RR_RGBA(0x55,0x55,0xFF),
    RR_RGBA(0x55,0xFF,0x55),
    RR_RGBA(0x55,0xFF,0xFF),
    RR_RGBA(0xFF,0x55,0x55),
    RR_RGBA(0xFF,0x55,0xFF),
    RR_RGBA(0xFF,0xFF,0x55),
    RR_RGBA(0xFF,0xFF,0xFF),
};

extern std::vector<ck_page> m_Pages;
extern unsigned int MaxLines;

// Max number of bitmaps to load
RR_Image m_BitmapImages[MAX_IMAGES];
RR_Image m_BorderBitmap;

extern RR_Window local_window;
int m_pageOn = 0;
int m_imageSelected = -1;
bool m_editText = false;
bool m_cursorflash = false;
int m_cursorx = 0;
int m_cursory = 0;
bool m_showBmps = true;

RR_KeyCode lastKey;
int lastCode = 0;
bool upperCode = false;
RR_Mouse local_mouse, last_mouse;

RR_KeyCode asciikeys[] ={
	RRK_SPACE,
	RRK_EXCLAIM,
	RRK_QUOTEDBL,
	RRK_HASH,
	RRK_DOLLAR,
	RRK_PERCENT,
	RRK_AMPERSAND,
	RRK_QUOTE,
	RRK_LEFTPAREN,
	RRK_RIGHTPAREN,
	RRK_ASTERICK,
	RRK_PLUS,
	RRK_COMMA,
	RRK_MINUS,
	RRK_PERIOD,
	RRK_SLASH,
	RRK_0,
	RRK_1,
	RRK_2,
	RRK_3,
	RRK_4,
	RRK_5,
	RRK_6,
	RRK_7,
	RRK_8,
	RRK_9,
	RRK_COLON,
	RRK_SEMICOLON,
	RRK_LESS_THAN,
	RRK_EQUALS,
	RRK_GREATER_THAN,
	RRK_QUESTION_MARK,
	RRK_AT,
	RRK_A,
	RRK_B,
	RRK_C,
	RRK_D,
	RRK_E,
	RRK_F,
	RRK_G,
	RRK_H,
	RRK_I,
	RRK_J,
	RRK_K,
	RRK_L,
	RRK_M,
	RRK_N,
	RRK_O,
	RRK_P,
	RRK_Q,
	RRK_R,
	RRK_S,
	RRK_T,
	RRK_U,
	RRK_V,
	RRK_W,
	RRK_X,
	RRK_Y,
	RRK_Z,
	RRK_LEFT_BRACKET,
	RRK_BACKSLASH,
	RRK_RIGHT_BRACKET,
	RRK_CARET,
	RRK_UNDERSCORE,
	RRK_BACKQUOTE,
};

void EdGUI_HandleInput(){

	// The last mouse event
    last_mouse = local_mouse;
    // Get new mouse event
    local_mouse = RR_GetMouse(local_window);

    if(local_mouse.buttons[0].cur_state==1){
    }
    if(local_mouse.wheel_y > 0){
    }
    if(local_mouse.wheel_y < 0){
    }
    if(m_editText){
        for(int i = 0; i < 65; i++){
            if(local_window.keycodes[asciikeys[i]]){
                lastCode = asciikeys[i];
                upperCode = false;
                if(RR_GetKey(local_window,RRK_LSHIFT)){
                    upperCode = true;
                }
            }else{
                if(lastCode == asciikeys[i]){
                    i += ' ';
                    // Write the char
                    if(!upperCode){
                        i = tolower(i);
                    }
                    m_Pages.at(m_pageOn).lines[m_cursory][m_cursorx].ch = i;
                    m_cursorx += 1;
                    lastCode = 0;
                    break;
                }
            }
        }
    }

    if(RR_GetKey(local_window, RRK_UP)){
        lastKey = RRK_UP;
    }else if(RR_GetKey(local_window, RRK_DOWN)){
        lastKey = RRK_DOWN;
    }else if(RR_GetKey(local_window, RRK_LEFT)){
        lastKey = RRK_LEFT;
    }else if(RR_GetKey(local_window, RRK_RIGHT)){
        lastKey = RRK_RIGHT;
    }else if (RR_GetKey(local_window, RRK_DELETE)){
        lastKey = RRK_DELETE;
    }else if (RR_GetKey(local_window, RRK_TAB)){
        lastKey = RRK_TAB;
    }else if (RR_GetKey(local_window, RRK_E)){
        lastKey = RRK_E;
    }else if (RR_GetKey(local_window, RRK_S)){
        lastKey = RRK_S;
    }else if (RR_GetKey(local_window, RRK_K)){
        lastKey = RRK_K;
    }else if (RR_GetKey(local_window, RRK_ESCAPE)){
        lastKey = RRK_ESCAPE;
    }else if (RR_GetKey(local_window, RRK_RETURN)){
        lastKey = RRK_RETURN;
    }else if (RR_GetKey(local_window, RRK_BACKSPACE)){
        lastKey = RRK_BACKSPACE;
    }else if (RR_GetKey(local_window, RRK_G)){
        lastKey = RRK_G;
    }else if (RR_GetKey(local_window, RRK_B)){
        lastKey = RRK_B;
    }else{
        if(m_editText){
            if(lastKey == RRK_RETURN){
                // Move all lines below down??
                for(int i = MAX_LINES-2; i >= m_cursory && i > 0; i--){
                    for(int e = 0; e < MAX_LINE; e++){
                        m_Pages.at(m_pageOn).lines[i+1][e] = m_Pages.at(m_pageOn).lines[i][e];
                    }
                }
            }
            if(lastKey == RRK_BACKSPACE){
                // Remove the character
                m_cursorx -= 1;
                if(m_cursorx < 0) m_cursorx = 0;
                m_Pages.at(m_pageOn).lines[m_cursory][m_cursorx].ch = ' ';
            }
            if(lastKey == RRK_DELETE){
                for(int i = m_cursory+1; i < MAX_LINES ; i++){
                    for(int e = 0; e < MAX_LINE; e++){
                        m_Pages.at(m_pageOn).lines[i-1][e] = m_Pages.at(m_pageOn).lines[i][e];
                    }
                }
            }
            if(lastKey == RRK_UP){
                m_cursory -= 1;
            }
            if(lastKey == RRK_DOWN){
                m_cursory += 1;
            }
            if(lastKey == RRK_RIGHT){
                m_cursorx += 1;
            }
            if(lastKey == RRK_LEFT){
                m_cursorx -= 1;
            }
            if(lastKey == RRK_ESCAPE){
                m_editText = false;
                m_showBmps = true;
            }
            if(m_cursorx < 0 ) m_cursorx = 0;
            if(m_cursory < 0 ) m_cursory = 0;
            if(m_cursorx >= MAX_LINE){
                m_cursorx = 0;
                m_cursory += 1;
            }
            if(m_cursory >= MaxLines){
                m_cursory = MaxLines-1;
            }
        }else{
            if(lastKey == RRK_RIGHT){
                m_pageOn += 1;
            }
            if(lastKey == RRK_LEFT){
                m_pageOn -= 1;
            }
            if(lastKey == RRK_E){
                if(!m_Pages.size()){
                    m_Pages.push_back(ck_page());
                } 
                m_editText = true;
            }
            if(lastKey == RRK_G){
                m_showBmps = !m_showBmps;
            }
            if(lastKey == RRK_B){
                page_block block;
                block.x = local_mouse.x; block.y = local_mouse.y;
                block.w = block.h = 1;
                block.color = 0;
                block.blockx = block.x>>3; block.blocky = block.y>>3;
                m_Pages.at(m_pageOn).blocks.push_back(block);
            }
            if(lastKey == RRK_S){
                SavePages();
            }
            if(lastKey == RRK_K){
                ExportPages(); // Export the pages
            }
            if(lastKey == RRK_DELETE){
                if(m_Pages.size()) 
                    m_Pages.erase(m_Pages.begin()+m_pageOn); // Remove the page
            }
            if(lastKey == RRK_TAB){
                // Copy the page
                if(m_Pages.size()) {
                    m_Pages.insert(m_Pages.begin()+m_pageOn,m_Pages.at(m_pageOn));
                }

            }
        }
        lastKey = RRK_NONE;
    }
    if(m_pageOn < 0) m_pageOn = 0;
    if(m_pageOn >= m_Pages.size()) m_pageOn = m_Pages.size()-1;
};

void EdGUI_Draw(){

    // Clear the screen
    RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(120,0,0), 0));
    RR_BlitLine(local_window, 320,0, 320, local_window.screen_height, RR_Pixel(RR_RGBA(0,0,0)));
    RR_BlitLine(local_window, 321,0, 321, local_window.screen_height, RR_Pixel(RR_RGBA(255,255,0)));
    RR_BlitLine(local_window, 322,0, 322, local_window.screen_height, RR_Pixel(RR_RGBA(0,0,0)));

    RR_BlitImage(local_window, m_BorderBitmap, 0, 0);

    // Draw an 8x8 grid
    RR_SetAlphaBlend(local_window, RR_BLEND_PIXELS);
    for(int i = 0; i < m_BorderBitmap.width; i+= 8){
        RR_BlitLine(local_window, i, 0, i, m_BorderBitmap.height, RR_Pixel(RR_RGBA(0,0,0, 128)));
    }
    for(int i = 0; i < m_BorderBitmap.height; i+= 8){
        RR_BlitLine(local_window, 0, i, m_BorderBitmap.width, i, RR_Pixel(RR_RGBA(0,0,0, 128)));
    }
    RR_SetAlphaBlend(local_window, RR_BLEND_NONE);
    // Write the page
    if(m_Pages.size()){
        if(m_showBmps){
            int imgcount = 0;
            int last_hover = -1;
            if(m_imageSelected == -1){
                for(page_img &img : m_Pages.at(m_pageOn).imgs){
                    if(local_mouse.x > img.blockx*8 && local_mouse.x < (img.blockx*8)+m_BitmapImages[img.imgid].width){
                        if(local_mouse.y > img.blocky*8 && local_mouse.y < (img.blocky*8)+m_BitmapImages[img.imgid].height){
                            last_hover = imgcount;
                        }
                    }
                    imgcount++;
                }
            }
            imgcount = 0;
            for(page_img &img : m_Pages.at(m_pageOn).imgs){
                RR_BlitImage(local_window, m_BitmapImages[img.imgid], img.blockx*8, img.blocky*8);

                RR_Color selcol = RR_RGBA(255,255,0);
                if((m_imageSelected == -1 && last_hover == imgcount) || m_imageSelected == imgcount){
                    if(RR_GetKey(local_window,RRK_EQUALS) || RR_GetKey(local_window,RRK_PLUS)){
                        // Increase time?
                        img.time += 1;
                    }
                    if(RR_GetKey(local_window,RRK_MINUS)){
                        // Increase time?
                        img.time -= 1;
                        if(img.time < 0){
                            img.time = 0;
                        }
                    }
                    if(last_mouse.buttons[2].cur_state && !local_mouse.buttons[2].cur_state ){
                        // Remove the image
                        m_Pages.at(m_pageOn).imgs.erase(m_Pages.at(m_pageOn).imgs.begin()+imgcount);
                        last_hover = -1;
                        continue;
                    }
                    if(local_mouse.buttons[0].cur_state ){
                        selcol = RR_RGBA(255,0,0);
                        img.x += local_mouse.x - last_mouse.x;
                        img.y += local_mouse.y - last_mouse.y;
                        img.blockx = img.x>>3;
                        img.blocky = img.y>>3;
                        m_imageSelected = imgcount;
                    }else{
                        m_imageSelected = -1;
                        selcol = RR_RGBA(0,255,0);
                    }
                }
                imgcount ++;
                RR_BlitBox(local_window, img.blockx*8, img.blocky*8, m_BitmapImages[img.imgid].width, m_BitmapImages[img.imgid].height, RR_Pixel(selcol));
            }
            // Never mind blocks?
            for(page_block &block : m_Pages.at(m_pageOn).blocks){
                RR_Pixel col = RR_Pixel(RR_RGBA(255,255,0));
                //RR_BlitRect(local_window, block.blockx*8, block.blocky*8, block.w*8, block.h*8, col);
            }
            if(last_hover != -1){
                EdWriteString("Image info",42*8,8,RR_Pixel(RR_RGBA(255,255,0)));
                EdWriteString("Time: "+std::to_string(m_Pages.at(m_pageOn).imgs.at(last_hover).time),42*8,16,RR_Pixel(RR_RGBA(255,255,0)));
            }
        }

        for(int y = 0; y < MAX_LINES; y++){
            for(int x = 0; x < MAX_LINE; x++){
                t_char ch = m_Pages.at(m_pageOn).lines[y][x];
                RR_Color col = EGA_Colors[ch.color];
                EdDrawChar((x*8)+8,(y*8)+8, ch.ch, RR_Pixel(col));
                if(m_editText){
                    col = EGA_Colors[ch.color^8];
                    if(x==m_cursorx && y==m_cursory)
                        EdDrawChar((x*8)+14,(y*8)+8, '|', RR_Pixel(col));
                }
            }
        }

        m_cursorflash = !m_cursorflash;

        if(m_editText){
            EdWriteString("Editing",8,20*8,RR_Pixel(RR_RGBA(255,255,0)));
        }else{
            // Write some info
            if(m_Pages.at(m_pageOn).timedpage){
                EdWriteString("Time Sub Page",8,20*8,RR_Pixel(RR_RGBA(255,255,0)));
            }
        }
    }else{
        EdWriteString("No Pages",80,20*8,RR_Pixel(RR_RGBA(255,255,0)));
    }
    EdWriteString("Page "+std::to_string(m_pageOn+1)+" of "+std::to_string(m_Pages.size()),80,20*8,RR_Pixel(RR_RGBA(255,255,0)));

    // Render the scene
    RR_RasterWindow(local_window);

};

#include "CK_FONT.h"

void EdDrawChar(int x, int y, char c, RR_Pixel pixel){
    for(int i = 0; i < 8; i++){
        for(int e = 0; e < 8; e+=2){
            int chrof = (c*8*4)+(i*4)+(e>>1);
            if(CK_FONT[chrof]&0xF)
                RR_Plot(local_window, x+e, y+i, pixel);
            if(CK_FONT[chrof]&0xF0)
                RR_Plot(local_window, x+e+1, y+i, pixel);
        }
    }
};

void EdWriteString(std::string str, int x, int y, RR_Pixel pixel){
    for(const char &c : str){
        EdDrawChar(x, y, c, pixel);
        x+=8;
    }
};
