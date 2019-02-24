#ifndef __SUPER_BULLET_H__
#define __SUPER_BULLET_H__

#include <iostream>
#include "Character\Bullet\Bullets.h"

class SuperBullets {
private:
	std::map<int, Bullets*> m_bullets;
public:
	SuperBullets();
	void addBullets(int key, Bullets *bullets);
	void nextBullet(int key, const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct);
	Bullets* getBullets(int key);

	void addParent(cocos2d::Layer *layer);
	void createPhysicsBody(b2World *world);
	void update(float deltaTime);
};

#endif