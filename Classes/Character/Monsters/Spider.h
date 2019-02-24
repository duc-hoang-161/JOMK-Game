#ifndef __SPIDER_H__
#define __SPIDER_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class Spider : public Monster {
private:
	static const int STATE_WALK;
	static const int STATE_WALKUP;
	static const int STATE_DIE;
	static const int STATE_IDLE;
public:
	Spider();

	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	
	~Spider();
};

#endif