//   ______   ______   _________  __          
//  /_____/\ /_____/\ /________/\/_/\         
//  \:::_ \ \\:::_ \ \\__.::.__\/\:\ \        
//   \:\ \ \ \\:\ \ \ \  \::\ \   \:\ \       
//    \:\ \ \ \\:\ \ \ \  \::\ \   \:\ \____  
//     \:\/.:| |\:\_\ \ \  \::\ \   \:\/___/\ 
//      \____/_/ \_____\/   \__\/    \_____\/ 
//        by mac


#ifndef __OGRO_H
#define __OGRO_H

/************************************************************
	Brute.H

	The MD2Enemy class

	Description: The BruteEnemy This enemy is considered "dumb",
			   but has a thicker coat of armor.
			   Adapted By: DOTL Dev Team

*************************************************************/

#include "enemy.h"

using namespace lcgl;

#define BRUTE_RADIUS_OF_AWARENESS 100.0f //how close you can get to an ogro before he notices you
#define BRUTE_ZIGZAG_ANGLE         90 //how much ogro will veer left or right when running away

class BruteEnemy : public Enemy
{
protected:
	void OnCollision(Object *collisionObject);
	int stateStart_, stateEnd_;	// state keyframe start/end
	int stateStart () const { return stateStart_; } //used by Ogro and Sod
	lcgl::Direction _direction;//private direction mac 4/3/14
	Sound _death; 
	
public:
	BruteEnemy(const ModelMD2& model,
				World* world, Player* p, float x=0.0f, float z=0.0f) 
		: Enemy (new InstanceMD2  (model, DEFAULT_ENTITY_INTERPOLATION), 
		world, 
		p, 
		BRUTE_RADIUS_OF_AWARENESS, 
		BRUTE_ZIGZAG_ANGLE,
		DEFAULT_ENTITY_SIZE,
		worldCoord (x, 0, z)
		), _death("sounds//dyinggroan.wav")
	{  
		stateStart_ = 0;
		stateEnd_ = 39;	
	}
	~BruteEnemy() {}
	BruteEnemy (const BruteEnemy&);		//intentionally not written
	void operator=(const BruteEnemy&);	//intentionally not written

	void OnPrepare();
	void OnDraw(const Player&);
};


#endif
