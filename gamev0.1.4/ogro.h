#ifndef __OGRO_H
#define __OGRO_H

/*
	OGRO.H

	The SodEnemy class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description: The OgroEnemy represents the Ogro model
			   enemy. This enemy is considered "dumb",
			   but has a thicker coat of armor.

*/

#include "enemy.h"

using namespace lcgl;

#define OGRO_RADIUS_OF_AWARENESS 100.0f //how close you can get to an ogro before he notices you
#define OGRO_ZIGZAG_ANGLE         90 //how much ogro will veer left or right when running away

class OgroEnemy : public Enemy
{
protected:
	void OnCollision(Object *collisionObject);
	int stateStart_, stateEnd_;	// state keyframe start/end
	int stateStart () const { return stateStart_; } //used by Ogro and Sod
	lcgl::Direction _direction;//private direction mac 4/3/14
	
public:
	OgroEnemy(const ModelMD2& model,
				World* world, Player* p, float x=0.0f, float z=0.0f) 
		: Enemy (new InstanceMD2  (model, DEFAULT_ENTITY_INTERPOLATION), 
		world, 
		p, 
		OGRO_RADIUS_OF_AWARENESS, 
		OGRO_ZIGZAG_ANGLE,
		DEFAULT_ENTITY_SIZE,
		worldCoord (x, 0, z)
		) 
	{  
		stateStart_ = 0;
		stateEnd_ = 39;	
	}
	~OgroEnemy() {}
	OgroEnemy (const OgroEnemy&);		//intentionally not written
	void operator=(const OgroEnemy&);	//intentionally not written

	void OnPrepare();
	void OnDraw(const Player&);
};


#endif
