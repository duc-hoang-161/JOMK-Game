#include "Items\EquipItem.h"

EquipItem::EquipItem()
{
	damage = 0;
}


int EquipItem::getDefense()
{
	return this->defense;
}
void EquipItem::setDefense(int defense)
{
	this->defense = defense;
}

std::string EquipItem::showInfo()
{

	std::string description = "";
	description += "(" + this->nameItem + ")";
	description += "\n" + this->infoItem;
	if (this->price > 0)
		description += CCString::createWithFormat("\nPrice: %d", this->price)->getCString();
	description += CCString::createWithFormat("\nDefense: %d", this->defense)->getCString();
	description += CCString::createWithFormat("\nSprint: %d", this->sprint)->getCString();
	return description;
}

int EquipItem::getCurrentLvl()
{
	return this->currentLvl;
}
void EquipItem::setCurrentLvl(int currentLvl)
{
	this->currentLvl = currentLvl;
}
int EquipItem::getNextLvl()
{
	return this->nextLvl;
}
void EquipItem::setNextLvl(int nextLvl)
{
	this->nextLvl = nextLvl;
}

int EquipItem::getSprint()
{
	return this->sprint;
}
void EquipItem::setSprint(int sprint)
{
	this->sprint = sprint;
}
int EquipItem::getDamage()
{
	return this->damage;
}
void EquipItem::setDamage(int damage)
{
	this->damage = damage;
}
int EquipItem::getPositionEquip()
{
	return this->positionEquip;
}
void EquipItem::setPositionEquip(int positionEquip)
{
	this->positionEquip = positionEquip;
}