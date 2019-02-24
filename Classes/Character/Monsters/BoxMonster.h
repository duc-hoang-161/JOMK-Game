#ifndef __BOXMONSTER_H__
#define __BOXMONSTER_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class BoxMonster : public Monster {
private:
	static const int STATE_WALK;
	static const int STATE_ATTACK;
	static const int STATE_IDLE;
public:
	BoxMonster();

	void update(float deltaTime);
	bool attack(LivingObject *lObj);
	
	
	~BoxMonster();
};

#endif