#ifndef __FROGMONSTER_H__
#define __FROGMONSTER_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class FrogMonster : public Monster {
private:
	static const int STATE_DIE;
	static const int STATE_ATTACK;
	static const int STATE_IDLE;
	Bullets *m_bullets;
	bool m_canShot;
public:
	FrogMonster();

	void addParent(Layer *layer);
	void createPhysicsBody(b2World *physicsWorld);

	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	
	void shot();

	~FrogMonster();
};

#endif