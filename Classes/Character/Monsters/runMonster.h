#ifndef __RUNMONSTER_H__
#define __RUNMONSTER_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class RunMonster : public Monster {
private:
	static const int STATE_WALK;
	static const int STATE_DIE;
	
public:
	RunMonster();

	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	bool isMoveLeft;
	Vec2 originalPosition;
	~RunMonster();
};

#endif