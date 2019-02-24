#ifndef __BUTLLETS_H__
#define __BUTLLETS_H__

#include <iostream>
#include <vector>
#include <string>
#include "Character\Bullet\Bullet.h"
#include "cocos2d.h"
#include "Box2D\Box2D.h"

class Bullets {
private:
	std::vector<Bullet*> m_bullets;
public:
	Bullets(int number, const std::string &file);
	bool nextBullet(const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct);
	void addCollisionCondition(int bitmask, const std::function<void(Bullet*, LivingObject*)> &func);
	void setMaxDistance(int maxDistance);
	void setTime(int ms);

	void setAnchorPoint(Vec2 vec2);

	void addParent(Layer *layer);
	void createPhysicsBody(b2World *physicsWorld);
	void update(float deltaTime);
	void setFilterData(b2Filter filter);
	void setUserData(void *data);
	void setAnimateForever(bool v);

	~Bullets();
};

#endif