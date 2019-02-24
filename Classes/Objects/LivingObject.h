#ifndef __LIVING_OBJECT_H__
#define __LIVING_OBJECT_H__

#include <iostream>
#include <algorithm>
#include "Box2D\Box2D.h"
#include <map>
#include "cocos2d.h"

class LivingObject {
private:
	b2Body **m_pPhysicsBody;	// cơ thể vật lý
	cocos2d::Sprite **m_pSprite; // sprite
	int *m_pHP;					// máu
	int *m_pMaxHP;				// máu tối đa
	int *m_pArmor;				// giáp
	int *m_pRunStep;			// tốc độ di chuyển

	std::map<int, std::set<LivingObject*>> m_attackedObject;
public:
	void addAttackedObject(int damageType, LivingObject *lObj);
	void removeAttackedObject(int damageType, LivingObject *lObj);
	void clearAttackedObject(int damageType);
	bool canAttack(int damageType, LivingObject *lObj);

	LivingObject();
	void pRegisterPhysicsBody(b2Body **physicsBody);
	void pRegisterHP(int *HP);
	void pRegisterMaxHP(int *maxHP);
	void pRegisterArmor(int *armor);
	void pRegisterRunStep(int *runStep);
	void pRegisterSprite(cocos2d::Sprite **sprite);

	b2Vec2 pGetPosition();
	int pGetRunStep();
	void pChangePosition(b2Vec2 deltaPosition);
	void pReceiveDamage(int damage);
	void pChangeRunStep(int time, float percent);
};

#endif