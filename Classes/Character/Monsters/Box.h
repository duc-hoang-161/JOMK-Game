#ifndef __BOX_H__
#define __BOX_H__

#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"

class Box : public Monster {
private:
	
public:
	Box();

	void update(float deltaTime);
	~Box();
	
};

#endif