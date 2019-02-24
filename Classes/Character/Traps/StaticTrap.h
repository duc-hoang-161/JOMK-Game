#ifndef _STATICTRAP_H_
#define _STATICTRAP_H_

#include "Character\Traps\Trap.h"
class StaticTrap : public Trap {
private:
	int m_damage;
public:
	StaticTrap();
	 
	virtual void collision(LivingObject *lObj, void *data = NULL);
	void createPhysicsBody(b2World *physicsWorld);
	void update(float deltaTime);

	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

};

#endif