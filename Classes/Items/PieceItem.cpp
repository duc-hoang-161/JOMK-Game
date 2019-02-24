#include "Items\PieceItem.h"

PieceItem::PieceItem()
{
}


void PieceItem::setKey(int key)
{
	this->key = key;
}
int PieceItem::getKey()
{
	return this->key;
}


void PieceItem::setValue(int value)
{
	this->value = value;
}

int PieceItem::getValue()
{
	return this->value;
}


std::string PieceItem::showInfo()
{
	std::string description = "";
	description += "(" + this->nameItem + ")";
	description += "\n" + this->infoItem;
	return description;
}

