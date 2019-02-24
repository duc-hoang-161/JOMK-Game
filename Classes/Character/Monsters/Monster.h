#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Objects\DynamicGameObject.h"
#include "ui\UILoadingBar.h"
#include "Objects\LivingObject.h"
#include "Character\MainCharacter\MainCharacter.h"
#include "Character\Items\Item.h"

class MainCharacter;

class Monster : public DynamicGameObject, public LivingObject {
private:

	void showGold();
protected:

	int m_traceDistance;

	int m_HP;
	int m_maxHP;
	int m_damage;
	int m_runStep;
	int m_gold;
	std::vector<Item*> m_items;

	MainCharacter *m_mainCharacter;
public:
	Sprite *m_healthBarBackground;
	ui::LoadingBar *m_healthBar;
	static const int REGION_BODY;
	static const int REGION_DAMAGE;

	Monster(string animationFileDirect);

	virtual void setAttribute(string jsonFile);
	virtual void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	virtual void moveLeft(float deltaTime);
	virtual void moveRight(float deltaTime);

	// get,set
	void destroy();
	void addMainCharacter(MainCharacter *mc);
	void addItem(Item *item);

	// override method of GameObject class
	virtual void addParent(Layer *layer);
	virtual void update(float deltaTime);	
	virtual void createPhysicsBody(b2World *physicsWorld);
	virtual string className() { return "Monster"; }

	virtual bool attack(LivingObject *lObj);
	void updateHealthBar();

	~Monster();
};

#endif