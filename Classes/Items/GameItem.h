#include "cocos2d.h"
using namespace cocos2d;

#ifndef GAME_ITEM_H
#define GAME_ITEM_H

class GameItem
{
protected:
	int idType;
	int idItem;
	std::string nameItem;
	std::string infoItem;
	int price;
	bool isDrop;
	std::string srcImage;
	std::string srcImageDrop;
public:
	GameItem();
	GameItem(int idType, int idItem, std::string nameItem, std::string infoItem, int price, bool isDropItem, std::string srcImage, std::string srcImageDrop);
	//get function
	int getTypeItem();
	int getIdItem();
	std::string getNameItem();
	std::string getInfoItem();
	int getPrice();
	bool isDropItem();
	std::string getSrc();
	std::string getSrcInGame();

	//set function
	void setTypeItem(int idType);
	void setIdItem(int idItem);
	void setNameItem(std::string nameItem);
	void setInfoItem(std::string infoItem);
	void setPrice(int price);
	void setDropItem(bool isDrop);
	void setSrcImage(std::string src);
	void setSrcDropImage(std::string src);

	//method
	virtual std::string showInfo() = 0;
};

#endif