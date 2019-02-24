#include "cocos2d.h"
#include "ui/UIButton.h"
#include "Items\GameItem.h"
#include "Import\ItemsConfig.h"
using namespace cocos2d;


#ifndef SLOT_H
#define SLOT_H

class Slot
{
protected:
	Layer* slotLayer;
	ui::Button* slotButton;
	Sprite* itemImage;
	Sprite* m_selected;
	GameItem* item;
	int index;
	bool m_isSelected;
	bool m_isAvailable;
	int typeSlot;
public:
	Slot();
	Slot(GameItem* item);
	Slot(int type);
	void addToParent(Layer* parentLayer, int zOrder, Vec2 position, Sprite* selected, int tag, std::function<void(int, bool)> callback);
	GameItem* getItem();
	void createSlot();
	void setItem(GameItem* item);
	void setEnable(bool isEnable);
	void updateDataSlot();
	void setSelected(bool isSelected);
	bool isSelected();
	void setEmpty();
	void setOpacity(int opacity);
	void setType(int typeSlot);
	static const int SLOT_ITEM = 1;
	static const int SLOT_PRICE = 2;
	static const int SLOT_SKILL = 3;
	void setChoose(bool isAvailable);
	Vec2 getPosition();
	bool isAvailable();
};

#endif