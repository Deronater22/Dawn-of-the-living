
//   ______   ______   _________  __          
//  /_____/\ /_____/\ /________/\/_/\         
//  \:::_ \ \\:::_ \ \\__.::.__\/\:\ \        
//   \:\ \ \ \\:\ \ \ \  \::\ \   \:\ \       
//    \:\ \ \ \\:\ \ \ \  \::\ \   \:\ \____  
//     \:\/.:| |\:\_\ \ \  \::\ \   \:\/___/\ 
//      \____/_/ \_____\/   \__\/    \_____\/ 
//        by DNT


#pragma once
#include "gui.h"

/***********************
Bite.h
players secondary weapon
adapted by Dotl Dev Team 

***********************/

class Bite
{
public:
	Bite::Bite(const ModelMD2& biteModel, const Texture2D& explosionTexInit, World* world, const Direction& newDirection);
	Bite(void);
	~Bite(void);

	bool _isJawsClosed;
	GUI* JawsClosed;
};
