#ifndef __ITEM_H__
#define __ITEM_H__

#include "Objects\GameObject.h"

class Item : public GameObject {
private:
	int m_type;
	int m_id;

	bool m_isPicked;
public:
	Item(int type, int id);	
	Item();
	void createPhysicsBody(b2World *world);
	std::string className() { return "Item"; }

	void drop(cocos2d::Vec2 position);
	void picked();
	virtual void addToPack();

	void update(float deltaTime);
};

#endif