#include "Objects\GameObject.h"
#include <iostream>
#include <string>
#include <cocos2d.h>

class RockFall : public GameObject {
public:
	RockFall(std::string file);
	void update(float deltaTime);
	void createPhysicsBody(b2World *physicsWorld);
	std::string className() { return "RockFall"; }
	/*vector<Sprite*> m_rocks;*/
	void Fall();
};