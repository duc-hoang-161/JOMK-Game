#ifndef __TRAPMOVEDOWN_H__
#define __TRAPMOVEDOWN_H__

#include "Objects\GameObject.h"

class TrapMoveDown : public GameObject {
public:
	TrapMoveDown(std::string linkImg);
	void update(float datatime);
	void createPhysicBody(b2World *physicsWork);
	std::string className()
	{
		return "TrapMoveDown";
	}	

};

#endif