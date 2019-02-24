#include "Items\MagicItem.h"

MagicItem::MagicItem()
{
	this->damage = 0;
}


//get function
int MagicItem::getManaToUse()
{
	return this->manaToUse;
}

int MagicItem::getDamage()
{
	return this->damage;
}

bool MagicItem::isSkill()
{
	return this->m_isSkill;
}

void MagicItem::setManaToUse(int manaToUse)
{
	this->manaToUse = manaToUse;
}

void MagicItem::setDamage(int damage)
{
	this->damage = damage;
}

void MagicItem::setSkill(bool _isSkill)
{
	this->m_isSkill = _isSkill;
}

std::string MagicItem::showInfo()
{
	std::string description = "";
	description = "(" + this->getNameItem() + ")";
	description += "\n" + this->getInfoItem();
	if (this->price > 0)
	{
		description += CCString::createWithFormat("\nPrice: %d GOLD", this->price)->getCString();
	}
	if (this->manaToUse > 0)
	{
		description += CCString::createWithFormat("\nMana to use: %d", this->manaToUse)->getCString();
	}
	if (this->damage > 0 && idItem > 14)
	{
		description += CCString::createWithFormat("\nDamage: %d", this->damage)->getCString();
	}
	if (m_isSkill)
	{
		int point = 0;
		if (idItem >= 20)
		{
			point = idItem - 19;
		}else
		if (idItem >= 15)
		{
			point = idItem - 14;
		}else
		if (idItem >= 9)
		{
			point = idItem - 8;
		}else
		if (idItem >= 5)
		{
			point = idItem - 4;
		}
		description += CCString::createWithFormat("\nMagic Point: %d", point)->getCString();
	}
	return description;
}

