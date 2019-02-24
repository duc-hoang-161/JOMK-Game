#ifndef __FALLING_TRAP_2_H__
#define __FALLING_TRAP_2_H__

#include "Objects\GameObject.h"
#include "Character\Traps\Trap.h"
#include <iostream>
#include <string>
#include <cocos2d.h>

class FallingTrap2 : public Trap {
private:
	bool m_isFall;
public:
	FallingTrap2();
	virtual void collision(LivingObject *lObj, void *data = NULL);
	void update(float deltaTime);

	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);
};

#endif