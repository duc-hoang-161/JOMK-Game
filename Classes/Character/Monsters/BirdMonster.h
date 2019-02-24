#ifndef __BIRDMONSTER_H__
#define __BIRDMONSTER_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class BirdMonster : public Monster {
private:
	static const int STATE_DIE;
	static const int STATE_ATTACK;
	static const int STATE_IDLE;
	static const int STATE_WALK;
public:
	BirdMonster();
	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);


	
	
	~BirdMonster();
};

#endif