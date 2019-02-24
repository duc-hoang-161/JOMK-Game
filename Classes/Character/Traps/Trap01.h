#ifndef __Trap01_H__
#define __Trap01_H__

#include "Objects\GameObject.h"

class Trap01 : public GameObject {
public:
	Trap01(std::string linkImg);
	void update(float datatime);
	void createPhysicBody(b2World *physicsWork);
	std::string className()
	{
		return "Trap01";
	}
	
	void setVisble(bool v);

	void move();

};

#endif