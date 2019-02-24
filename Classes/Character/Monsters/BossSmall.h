#ifndef __BOSSSMALL_H__
#define __BOSSSMALL_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"


class BossSmall : public Monster {
private:
	static const int STATE_IDLE;
	static const int STATE_WALK;
	static const int STATE_ATTACK;
	static const int STATE_DIE;

	Bullets *m_bullets;
	bool m_canShot;
public:
	BossSmall();
	
	void addParent(Layer *layer);
	void createPhysicsBody(b2World *physicsWorld);

	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	void shot();

	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);

	~BossSmall();
};

#endif