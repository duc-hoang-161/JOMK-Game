#ifndef __LAND_H__
#define __LAND_H__

#include "Character\Traps\Trap.h"

class MovingTrap : public Trap {
private:
	int m_distance;
	int m_orientation;	// 1: horizontal, 0: vertical
public:
	MovingTrap();

	virtual void collision(LivingObject *lObj, void *data = NULL);
	void createPhysicsBody(b2World *physicsWorld);
	void update(float deltaTime);

	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	void moveForever();
};

#endif