#ifndef __GUN_TRAP_H__
#define __GUN_TRAP_H__

#include "Objects\GameObject.h"
#include "Character\Traps\Trap.h"
#include <iostream>
#include <string>
#include <cocos2d.h>
#include "Helper\Helper.h"
#include "Character\Traps\FallingTrap2.h"

class GunTrap : public Trap {
private:
	int m_distanceToFall;
	int m_damage;
	Bullets *m_bullets;
	bool m_canShot;
	float time;
	FallingTrap2 *fallingTrap2;
public:
	GunTrap();


	virtual void collision(LivingObject *lObj, void *data = NULL);


	void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);
	void shot();
	void addParent(Layer *layer);
	void createPhysicsBody(b2World *physicsWorld);
	void update(float datatime);
};

#endif