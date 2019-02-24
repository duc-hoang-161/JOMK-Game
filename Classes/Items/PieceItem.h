#include "Items\GameItem.h"
#include "cocos2d.h"

using namespace cocos2d;

#ifndef PIECE_ITEM_H
#define PIECE_ITEM_H

class PieceItem : public GameItem
{
public:
	static const int ID_PIECE_ITEM = 444;
private:
	int key;
	int value;
public:
	PieceItem();
	void setKey(int key);
	int getKey();
	void setValue(int value);
	int getValue();
	std::string showInfo();
};

#endif