#ifndef __FALLING_TRAP_H__
#define __FALLING_TRAP_H__

#include "Objects\GameObject.h"
#include "Character\Traps\Trap.h"
#include <iostream>
#include <string>
#include <cocos2d.h>

class FallingTrap : public Trap {
private:
	int m_distanceToFall;
	int m_damage;
public:
	FallingTrap();
	
	virtual void collision(LivingObject *lObj, void *data = NULL);
	void update(float deltaTime);
	void Fall();

	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);
};

#endif