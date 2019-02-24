#include "Items\GameItem.h"

GameItem::GameItem()
{
	this->idType = 0;
	this->idItem = 0;
	this->nameItem = "";
	this->infoItem = "";
	this->price = 0;
	this->isDrop = false;
	this->srcImage = "";
	this->srcImageDrop = "";
}

GameItem::GameItem(int idType, int idItem, std::string nameItem, std::string infoItem, int price, bool isDropItem, std::string srcImage, std::string srcImageDrop)
{
	this->idType = idType;
	this->idItem = idItem;
	this->nameItem = nameItem;
	this->infoItem = infoItem;
	this->price = price;
	this->isDrop = isDropItem;
	this->srcImage = srcImage;
	this->srcImageDrop = srcImageDrop;
}

//get function
int GameItem::getTypeItem()
{
	return this->idType;
}
int GameItem::getIdItem()
{
	return this->idItem;
}
std::string GameItem::getNameItem()
{
	return this->nameItem;
}
std::string GameItem::getInfoItem()
{
	return this->infoItem;
}
int GameItem::getPrice()
{
	return this->price;
}
bool GameItem::isDropItem()
{
	return this->isDrop;
}
std::string GameItem::getSrc()
{
	return this->srcImage;
}
std::string GameItem::getSrcInGame()
{
	return this->srcImageDrop;
}

//set function
void GameItem::setTypeItem(int idType)
{
	this->idType = idType;
}
void GameItem::setIdItem(int idItem)
{
	this->idItem = idItem;
}
void GameItem::setNameItem(std::string nameItem)
{
	this->nameItem = nameItem;
}
void GameItem::setInfoItem(std::string infoItem)
{
	this->infoItem = infoItem;
}
void GameItem::setPrice(int price)
{
	this->price = price;
}
void GameItem::setDropItem(bool isDrop)
{
	this->isDrop = isDrop;
}
void GameItem::setSrcImage(std::string src)
{
	this->srcImage = src;
}
void GameItem::setSrcDropImage(std::string src)
{
	this->srcImageDrop = src;
}