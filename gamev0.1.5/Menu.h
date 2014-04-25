/************************************************************************/
/* 
	Menu.h
	Description: adds standard pause and information menus to the game 
	More to come later?
	written by: Michael Carey 4/17/14
*/
/************************************************************************/


#include "texture.h"
#include "billboardfont.h"
#pragma once


extern const int WINDOW_HI , WINDOW_WID; //window parameters

class Menu
{
public:
	Menu(void);
	~Menu(void);


	//menu draw functions
	void drawPauseIcon   ()const;
	void drawInfoIcon    ()const;
	void drawPauseMenu   ()const;
	void drawInfoPage    ()const;

private:

	//menu textures and fonts
	lcgl::BillboardFont  info_page;  //info page 
	lcgl::BillboardFont  info_font;  //smaller font for info page
	lcgl::Texture2D      info_icon;

	lcgl::Texture2D      pauseIcon_;
	lcgl::BillboardFont  pause_menu;
};
