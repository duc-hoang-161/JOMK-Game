#include "cocos2d.h"
#include "Slot.h"
#include "ui\UIScrollView.h"
#include "ui\UIButton.h"
using namespace cocos2d;

#ifndef MYGRIDVIEW_H
#define MYGRIDVIEW_H

class MyGridView
{
private:
	ui::ScrollView* scrollContainer;
	Layer* gridLayer;
	Vec2** gridPositon;
	Slot* listItemSlot;
	int totalSlot;
	int totalAvailableSlot;
	int column;
	int row;
	int verticalSpacing = 16;
	int horizontalSpacing = 16;
	std::vector<Vec2> listItems;
	std::function<void(Vec2, bool)> callback;
	Vec2 selectedSlot;
	int typeGrid;
	int indexSelected;
	void deleteSlotPosition();
public:
	MyGridView();
	MyGridView(int type);
	void setData(std::vector<Vec2> listItems);
	void setCallBackOnItemSelected(std::function<void(Vec2, bool)> callback);
	void setColumn(int column);
	void setSpaceVertical(int space);
	void setSpaceHorizontal(int space);
	void setTotalSlot(int totalSlot);
	void setAvailableSlot(int totalSlot);
	void expandSlot(int number);
	void deleteSelectedItem();
	void deleteItemByIndex(int index);
	bool isHaveSelected = false;
	void setParent(Layer* layer);
	GameItem* getItemSelected();
	void removeSlot(int id);
	void setSelectedSlot(int idSlot, bool isSlected);
	void pushItem(Vec2 item);
	void notifyDataChanged();
	void setEnable(bool isEnable);
	int getIndexItemSelected();
	Slot getSlotByIndex(int index);
	void setItemSlotByIndex(int index, int idType, int idItem);
	void setSelected(bool isSelected);
	static const int GRID_ITEM = 1;
	static const int GRID_PRICE = 2;
};

#endif
