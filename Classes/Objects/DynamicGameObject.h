#ifndef __DYNAMIC_GAME_OBJECT_H__
#define __DYNAMIC_GAME_OBJECT_H__

#include <iostream>
#include <string>
#include "json\rapidjson.h"
#include "json\document.h"
#include "Helper\PhysicsBodyManager.h"
#include <functional>
#include "Objects\GameObject.h"

using namespace std;


class DynamicGameObject : public GameObject {
protected:
		// action
	map<int, FiniteTimeAction*> m_actions;
	map<int, string> m_actionNames;
	map<int,int> m_actionPriority;

		// physics body manager
	PhysicsBodyManager *m_physicsBodyManager;
private:
	int m_countToUpdatePhysicsBody;
	SpriteFrame *m_currentFrame;
	bool m_flippedFrame;
public:
	DynamicGameObject(string fileName);
	void runAction(int actionTag);
	void runActionWithCallBack(int actionTag, CallFunc *callBack);
	
	virtual void update(float deltaTime);
	virtual void createPhysicsBody(b2World *physicsWorld);
	bool isRunningAction(int actionTag);
	string getActionName(int actionTag);
	void setActionPriority(int actionTag, int priority);

	~DynamicGameObject();
};

#endif