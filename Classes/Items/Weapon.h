#include "cocos2d.h"
#include "Items\GameItem.h"
using namespace cocos2d;

#ifndef WEAPON_H
#define WEAPON_H

class Weapon : public GameItem
{
public:
	static const int ID_WEAPON_ITEM = 222;
private:
	int weaponLvl;
	int nextLvlId;
	int damage;
public:
	Weapon();

	//get function
	int getWeaponLvl();
	int getnextLvlId();
	int getDamage();

	//set function
	void setWeaponLvl(int weaponLvl);
	void setnextLvlId(int nextLvlId);
	void setDamage(int damage);

	//define pure virtual function
	std::string showInfo();
};

#endif