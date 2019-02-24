#include "Items\Weapon.h"

Weapon::Weapon()
{
}

int Weapon::getWeaponLvl()
{
	return this->weaponLvl;
}

int Weapon::getnextLvlId()
{
	return this->nextLvlId;
}

int Weapon::getDamage()
{
	return this->damage;
}

void Weapon::setWeaponLvl(int weaponLvl)
{
	this->weaponLvl = weaponLvl;
}

void Weapon::setnextLvlId(int nextLvlId)
{
	this->nextLvlId = nextLvlId;
}

void Weapon::setDamage(int damage)
{
	this->damage = damage;
}

std::string Weapon::showInfo()
{
	std::string description = "";
	description += "(" + this->nameItem + ")";
	description += "\n" + this->infoItem;
	description += CCString::createWithFormat("\nPrice: %d", this->price)->getCString();
	description += CCString::createWithFormat("\nDamage: %d", this->damage)->getCString();
	return description;
}