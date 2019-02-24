#include "PhysicsContactManager.h"
#include "Character\Door\Door.h"

void PhysicsContactManager::BeginContact(b2Contact* contact)
{
	if (!MainCharacterAndGround(contact->GetFixtureA(), contact->GetFixtureB(), 1))
		if (!MainCharacterAndMonster(contact->GetFixtureA(), contact->GetFixtureB()))
			if (!BulletAndMonster(contact->GetFixtureA(), contact->GetFixtureB()))
				if (!MainCharacterAndTrap(contact->GetFixtureA(), contact->GetFixtureB()))
					if (!BulletAndMainCharacter(contact->GetFixtureA(), contact->GetFixtureB()))
						if (!MainCharacterAndItem(contact->GetFixtureA(), contact->GetFixtureB()))
							if (!MainCharacterAndDoor(contact->GetFixtureA(), contact->GetFixtureB()));
}

void PhysicsContactManager::EndContact(b2Contact* contact)
{
	if (!MainCharacterAndGround(contact->GetFixtureA(), contact->GetFixtureB(), -1))
		if (!MainCharacterAndTrap(contact->GetFixtureA(), contact->GetFixtureB(), -1));
}

bool PhysicsContactManager::MainCharacterAndMonster(b2Fixture *fA, b2Fixture *fB)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
		MainCharacter *mc = static_cast<MainCharacter*>(bodyA->GetUserData());
		if (fB->GetFilterData().categoryBits == BITMASK_MONSTER) {
			Monster *monster = static_cast<Monster*>(bodyB->GetUserData());

			if ((int)fA->GetUserData() == MainCharacter::REGION_BODY) {
				if (fB->GetUserData() == NULL || ((int)fB->GetUserData()) == Monster::REGION_DAMAGE) {
					if (monster->attack(dynamic_cast<LivingObject*>(mc)))
						mc->gethit();
				}
			}
			else if ((int)fA->GetUserData() == MainCharacter::REGION_DAMAGE) {
				if (fB->GetUserData() == NULL || ((int)fB->GetUserData()) == Monster::REGION_BODY) {
					mc->attack(dynamic_cast<LivingObject*>(monster));
				}
			}

			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
		MainCharacter *mc = static_cast<MainCharacter*>(bodyB->GetUserData());
		if (fA->GetFilterData().categoryBits == BITMASK_MONSTER) {
			Monster *monster = static_cast<Monster*>(bodyA->GetUserData());

			if ((int)fB->GetUserData() == MainCharacter::REGION_BODY) {
				if (fA->GetUserData() == NULL || ((int)fA->GetUserData()) == Monster::REGION_DAMAGE) {
					if (monster->attack(dynamic_cast<LivingObject*>(mc)))
						mc->gethit();
				}
			}
			else if ((int)fB->GetUserData() == MainCharacter::REGION_DAMAGE) {
				if (fA->GetUserData() == NULL || ((int)fA->GetUserData()) == Monster::REGION_BODY) {
					mc->attack(dynamic_cast<LivingObject*>(monster));
				}
			}

			return true;
		}
	}

	return false;
}

bool PhysicsContactManager::MainCharacterAndItem(b2Fixture *fA, b2Fixture *fB)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_ITEM) {
		Item *item = static_cast<Item*>(bodyA->GetUserData());
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			item->picked();
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_BULLET) {
		Item *item = static_cast<Item*>(bodyB->GetUserData());
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			item->picked();
			return true;
		}
	}

	return false;
}

bool PhysicsContactManager::MainCharacterAndDoor(b2Fixture *fA, b2Fixture *fB)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_DOOR) {
		Door *door = static_cast<Door*>(bodyA->GetUserData());
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			door->open();
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_DOOR) {
		Door *door = static_cast<Door*>(bodyB->GetUserData());
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			door->open();
			return true;
		}
	}

	return false;
}

bool PhysicsContactManager::BulletAndMainCharacter(b2Fixture *fA, b2Fixture *fB)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_BULLET) {
		Bullet *bullet = static_cast<Bullet*>(bodyA->GetUserData());
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			LivingObject *lObj = dynamic_cast<LivingObject*>(static_cast<MainCharacter*>(bodyB->GetUserData()));
			bullet->collision(fB->GetFilterData().categoryBits, lObj);
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_BULLET) {
		Bullet *bullet = static_cast<Bullet*>(bodyB->GetUserData());
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			LivingObject *lObj = dynamic_cast<LivingObject*>(static_cast<MainCharacter*>(bodyA->GetUserData()));
			bullet->collision(fA->GetFilterData().categoryBits, lObj);
			return true;
		}
	}

	return false;
}

bool PhysicsContactManager::MainCharacterAndTrap(b2Fixture *fA, b2Fixture *fB)
{
	if (!MainCharacterAndTrap(fA, fB, 1))
		return false;

	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_TRAP) {
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			MainCharacter *mc = static_cast<MainCharacter*>(bodyB->GetUserData());
			Trap *trap = static_cast<Trap*>(bodyA->GetUserData());
			trap->collision(mc);
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_TRAP) {
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			MainCharacter *mc = static_cast<MainCharacter*>(bodyA->GetUserData());
			Trap *trap = static_cast<Trap*>(bodyB->GetUserData());
			trap->collision(mc);
		}
	}
	
	return true;
}

bool PhysicsContactManager::MainCharacterAndTrap(b2Fixture *fA, b2Fixture *fB, int v)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_TRAP) {
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			static_cast<MainCharacter*>(bodyB->GetUserData())->addContactWithGround(v);
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_TRAP) {
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			static_cast<MainCharacter*>(bodyA->GetUserData())->addContactWithGround(v);
			return true;
		}
	}
	return false;
}

bool PhysicsContactManager::MainCharacterAndGround(b2Fixture *fA, b2Fixture *fB, int v)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_GROUND) {
		if (fB->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			static_cast<MainCharacter*>(bodyB->GetUserData())->addContactWithGround(v);
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_GROUND) {
		if (fA->GetFilterData().categoryBits == BITMASK_CHARACTER) {
			static_cast<MainCharacter*>(bodyA->GetUserData())->addContactWithGround(v);
			return true;
		}
	}
	return false;
}

bool PhysicsContactManager::BulletAndMonster(b2Fixture *fA, b2Fixture *fB)
{
	b2Body *bodyA = fA->GetBody();
	b2Body *bodyB = fB->GetBody();

	if (fA->GetFilterData().categoryBits == BITMASK_BULLET) {
		Bullet *bullet = static_cast<Bullet*>(bodyA->GetUserData());
		if (fB->GetFilterData().categoryBits == BITMASK_MONSTER) {
			LivingObject *lObj = dynamic_cast<LivingObject*>(static_cast<Monster*>(bodyB->GetUserData()));
			bullet->collision(fB->GetFilterData().categoryBits, lObj);
			return true;
		}
	}
	else if (fB->GetFilterData().categoryBits == BITMASK_BULLET) {
		Bullet *bullet = static_cast<Bullet*>(bodyB->GetUserData());
		if (fA->GetFilterData().categoryBits == BITMASK_MONSTER) {
			LivingObject *lObj = dynamic_cast<LivingObject*>(static_cast<Monster*>(bodyA->GetUserData()));
			bullet->collision(fA->GetFilterData().categoryBits, lObj);
			return true;
		}
	}

	return false;
}

PhysicsContactManager::~PhysicsContactManager()
{
}