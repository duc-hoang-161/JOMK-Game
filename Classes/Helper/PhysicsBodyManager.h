#ifndef __PHYSICS_BODY_MANAGER_H__
#define __PHYSICS_BODY_MANAGER_H__

#include <iostream>
#include <map>
#include <cocos2d.h>
#include <json\document.h>
#include "Box2D\Box2D.h"
#include "Helper\MyBodyParser.h"
#include <vector>

class PhysicsBodyManager {
private:
	std::map<cocos2d::SpriteFrame*, std::pair<FixtureDef*, FixtureDef*> > m_data;
	
	std::map<int, b2Filter> m_filters;
	b2World *m_physicsWorld;
	SpriteFrame *m_defaultFrame;
public:	
	PhysicsBodyManager(std::string directoryOfJsonFile);
	void add(SpriteFrame* spriteFrame, std::string key);
	void addFilter(int key, b2Filter value);
	void setPhysicsWorld(b2World *physicsWorld);
	void setDefaultFrame(SpriteFrame *sf);

	b2Body* get(SpriteFrame* spriteFrame, bool isFlippedX = false);
	b2Body* getDefault(bool isFlippedX = false);

	~PhysicsBodyManager();
};

#endif