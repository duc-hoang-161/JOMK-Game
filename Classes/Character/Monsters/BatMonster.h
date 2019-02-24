#ifndef __BATMONSTER_H__
#define __BATMONSTER_H__

#include "Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

#include "Character\Traps\Trap.h"
#include <iostream>
#include <string>


class BatMonster : public Trap {
private:
	int m_damage;
	int m_distance;
public:
	BatMonster();
	virtual void collision(LivingObject *lObj, void *data = NULL);
	void createPhysicsBody(b2World *physicsWorld);
	void update(float deltaTime);

	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);
	 void moveForever();
};

#endif