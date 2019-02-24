#ifndef __BULLET_H__
#define __BULLET_H__

#include "Objects\DynamicGameObject.h"
#include "Objects\LivingObject.h"
#include "cocos2d.h"
#include <iostream>
#include <map>
#include <functional>

class LivingObject;

class Bullet : public DynamicGameObject {
private:
	std::map<int, std::function<void(Bullet*, LivingObject*)>> m_collision;
	cocos2d::Vec2 m_direct;
	bool m_isAlive;
	bool m_isDestroy;
	bool m_animateForever;
	int m_maxDistance;

	static const int STATE_RUN;
	cocos2d::Vec2 m_startPosition;
	set<LivingObject*> m_attackedObject;

	int m_maximumTime;
	int t_startTime;

	void *m_userData;
public:
	Bullet(const std::string &animationFileDirect);
	
	void addCollisionCondition(int bitmask, const std::function<void(Bullet*, LivingObject*)> &func);
	void setDirect(const cocos2d::Vec2 &direct);
	void setMaxDistance(int maxDistance);
	void collision(int bitmask, LivingObject *lObj);
	void destroy();
	void run(const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct);
	void setAlive(bool v);
	void setFilterData(b2Filter filter);
	bool isAlive();
	void setTime(int time);
	void setUserData(void *data);
	void setAnimateForever(bool v);

	void createPhysicsBody(b2World *physicsWorld);
	void addParent(Layer *layer);
	void update(float deltaTime);
	std::string className() { return "Bullet"; }
};

#endif