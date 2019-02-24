#ifndef __PHYSICS_CONTACT_MANAGER_H__
#define __PHYSICS_CONTACT_MANAGER_H__

#include "cocos2d.h"
#include <iostream>
#include <map>
#include "Character\MainCharacter\MainCharacter.h"
#include "Character\Monsters\Monster.h"
#include "Character\Traps\Trap.h"
#include "Character\Items\Item.h"
#include "Box2D\Box2D.h"

class PhysicsContactManager : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	
	bool BulletAndMainCharacter(b2Fixture *fA, b2Fixture *fB);
	bool BulletAndMonster(b2Fixture *fA, b2Fixture *fB);
	
	bool MainCharacterAndItem(b2Fixture *fA, b2Fixture *fB);
	bool MainCharacterAndGround(b2Fixture *fA, b2Fixture *fB, int v);
	bool MainCharacterAndTrap(b2Fixture *fA, b2Fixture *fB, int v);
	bool MainCharacterAndMonster(b2Fixture *fA, b2Fixture *fB);
	bool MainCharacterAndTrap(b2Fixture *fA, b2Fixture *fB);
	bool MainCharacterAndDoor(b2Fixture *fA, b2Fixture *fB);

	~PhysicsContactManager();
};

#endif