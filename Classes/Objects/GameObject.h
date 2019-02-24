#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Define.h"
#include "Helper\b2Helper.h"
#include <iostream>
#include <map>
using namespace cocos2d;
using namespace std;

class GameObject {
	//static
private:
	static int COUNT;
public:	
	static GameObject* create(string fileName);

private:
	int m_tag;
protected:
	// Sprite
	Sprite *m_sprite;
	Layer *m_parent;
	// Physics Body
	b2Body *m_physicsBody;
	b2World *m_physicsWorld;
	b2Filter m_filterData;
	// Alive
	bool m_isAlive;
public:
	GameObject();
	virtual void addParent(Layer *parent);
	virtual void addParent(Layer *parent, int z);
	virtual void createPhysicsBody(b2World *physicsWorld);
	virtual void createPhysicsBody(b2World *physicsWorld, std::string data, std::string config, std::string key);
	virtual void setFilterData(b2Filter filter);
	void removePhysicsBody();

	int getTag();
	void setPosition(Vec2);
	Vec2 getPosition();
	Size getContentSize();
	Size getActualSize();	// after scale
	bool isVisible();
	void setVisible(bool);
	bool isIntersect(GameObject *p);
	Layer* getParent();
	bool isAlive();
	b2Body* getPhysicsBody();
	void setAnchorPoint(Vec2 vec2);
	virtual void update(float deltaTime);

	virtual string className() = 0;
	~GameObject();
};

#endif