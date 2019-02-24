#include "Items\GameItem.h"
#include "cocos2d.h"

#ifndef MAGIC_ITEM_H
#define MAGIC_ITEM_H

class MagicItem : public GameItem
{
public:
	static const int ID_MAGIC_ITEM = 111;

private:
	int manaToUse;
	int damage;
	bool m_isSkill;

public:
	MagicItem();


	//get function
	int getManaToUse();
	int getDamage();
	bool isSkill();
	//set function
	void setManaToUse(int manaToUse);
	void setDamage(int damage);
	void setSkill(bool _isSkill);
	//function when user using this item
	void actionUse();

	//define pure virtual from parent class
	std::string showInfo();
};

#endif