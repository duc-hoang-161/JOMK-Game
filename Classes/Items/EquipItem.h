#include "Items\GameItem.h"
#include "cocos2d.h"
using namespace cocos2d;

#ifndef EQUIP_ITEM_H
#define EQUIP_ITEM_H

class EquipItem : public GameItem
{
public:
	static const int ID_EQUIP_ITEM = 333;
private:
	int currentLvl;
	int nextLvl;
	int defense;
	int sprint;
	int damage;
	int positionEquip;
public:
	EquipItem();
	int getCurrentLvl();
	void setCurrentLvl(int currentLvl);
	int getNextLvl();
	void setNextLvl(int nextLvl);
	int getDefense();
	void setDefense(int defense);
	int getSprint();
	void setSprint(int sprint);
	int getDamage();
	void setDamage(int damage);
	int getPositionEquip();
	void setPositionEquip(int positionEquip);
	std::string showInfo();
};

#endif