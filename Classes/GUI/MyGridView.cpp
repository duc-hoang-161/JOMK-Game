#include"MyGridView.h"
MyGridView::MyGridView()
{
	scrollContainer = NULL;
	gridLayer = NULL;
	gridPositon = NULL;
	listItemSlot = NULL;
	this->typeGrid = GRID_ITEM;
	//callback = NULL;
}

MyGridView::MyGridView(int type)
{
	scrollContainer = NULL;
	gridLayer = NULL;
	gridPositon = NULL;
	listItemSlot = NULL;
	switch (type)
	{
		case GRID_PRICE:
		{
			this->typeGrid = GRID_PRICE;
			break;
		}
		default:
		{
			this->typeGrid = GRID_ITEM;
			break;
		}
	}
	this->typeGrid = type;
}

void MyGridView::setParent(Layer* layer)
{
	//create content containner
	scrollContainer = ui::ScrollView::create();
	scrollContainer->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollContainer->setBackGroundColor(Color3B::BLUE);
	scrollContainer->setContentSize(layer->getContentSize());
	scrollContainer->setScrollBarAutoHideEnabled(true);
	scrollContainer->setScrollBarAutoHideTime(-1);
	scrollContainer->setClippingEnabled(true);
	scrollContainer->setAnchorPoint(Vec2(0, 0));
	scrollContainer->setLayoutType(ui::Layout::Type::VERTICAL);
	scrollContainer->setInnerContainerSize(Size(layer->getContentSize().width, layer->getContentSize().height * 2));
	scrollContainer->setPosition(Vec2::ZERO);
	gridLayer = Layer::create();
	gridLayer->setContentSize(Size::ZERO);
	gridLayer->setPosition(Vec2(scrollContainer->getInnerContainerSize().width / 2 - gridLayer->getContentSize().width / 2, scrollContainer->getInnerContainerSize().height / 2 - gridLayer->getContentSize().height / 2));
	scrollContainer->addChild(gridLayer, 4);
	layer->addChild(scrollContainer);
}

void MyGridView::setData(std::vector<Vec2> listItems)
{
	deleteSlotPosition();
	this->listItems = listItems;
}

void MyGridView::setTotalSlot(int totalSlot)
{
	deleteSlotPosition();
	this->totalSlot = totalSlot;
}

void MyGridView::setAvailableSlot(int availableSlot)
{
	deleteSlotPosition();
	this->totalAvailableSlot = availableSlot;
}

void MyGridView::expandSlot(int number)
{
	if (this->totalAvailableSlot < this->totalSlot && this->totalAvailableSlot + number <= this->totalSlot)
	{
		while (number != 0)
		{
			number--;
			this->listItemSlot[++totalAvailableSlot - 1].setEnable(true);
		}
	}
}

void MyGridView::deleteSlotPosition()
{
	if (gridPositon != NULL)
	{
		for (int i = 0; i < totalSlot / this->column; i++)
		{
			delete[] gridPositon[i];
		}
		delete[] gridPositon;
		gridLayer->removeAllChildren();
	}
	gridPositon = NULL;
}

void MyGridView::setColumn(int column)
{
	
	this->column = column;
	
}

GameItem* MyGridView::getItemSelected()
{
	if (this->isHaveSelected)
	{
		int index = this->selectedSlot.x*this->column + this->selectedSlot.y + 1;
		if (index <= listItems.size())
		{
			return ItemsConfig::getInstance()->getItem(listItems[index - 1].x, listItems[index - 1].y);
		}
		return NULL;
	}
	return NULL;
}

void MyGridView::removeSlot(int id)
{
	gridLayer->removeChildByTag(id);
}

void MyGridView::setSelectedSlot(int idSlot, bool isSelected)
{
	this->selectedSlot.x = (int)idSlot / 10;
	this->selectedSlot.y = (int)idSlot % 10;
	this->isHaveSelected = isSelected;
	if (isSelected)
	{
		for (int i = 0; i < totalSlot; i++)
		{
			if (i != (this->selectedSlot.x*this->column + this->selectedSlot.y))
				listItemSlot[i].setSelected(false);
		}
	}
	//if (this->callback != NULL)
	{
		this->callback(this->selectedSlot, isSelected);
	}
}

void MyGridView::deleteSelectedItem()
{
	if (this->isHaveSelected)
	{
		int index = this->selectedSlot.x*this->column + this->selectedSlot.y;
		//move slot to previous a slot
		for (int i = index; i < listItems.size() - 1; i++)
		{
			this->listItemSlot[i].setItem(ItemsConfig::getInstance()->getItem(listItems[i + 1].x, listItems[i + 1].y));
		}
		//un-select slot
		this->listItemSlot[index].setSelected(false);
		//clear finnal slot
		this->listItemSlot[listItems.size() - 1].setEmpty();
		//delete item from list
		this->listItems.erase(this->listItems.begin() + index);
		//change to un-select state if pack is empty or delete the end of list
		if (listItems.size() == 0 || index >= listItems.size())
			this->isHaveSelected = false;

	}
}

void MyGridView::pushItem(Vec2 item)
{
	if (this->listItems.size() - this->totalAvailableSlot > 0)
	{
		this->listItems.push_back(item);
		this->listItemSlot[this->listItems.size() - 1].setItem(ItemsConfig::getInstance()->getItem(item.x, item.y));
		log("da day vao, list chua %d", this->listItems.size());
	}
	else
	{
		log("list day");
	}
}


void MyGridView::setCallBackOnItemSelected(std::function<void(Vec2, bool)> callback)
{
	//if (this->callback != NULL)
	{
		this->callback = callback;
	}
}

void MyGridView::setEnable(bool isEnable)
{
	for (int i = 0; i < this->totalAvailableSlot; i++)
	{
		listItemSlot[i].setEnable(isEnable);
	}
}

void MyGridView::setSpaceVertical(int space)
{
	this->verticalSpacing = space;
}
void MyGridView::setSpaceHorizontal(int space)
{
	this->horizontalSpacing = space;
}

int MyGridView::getIndexItemSelected()
{
	if (this->getItemSelected() != NULL)
	{
		return this->selectedSlot.x*this->column + this->selectedSlot.y;
	}
	return -1;
}

void MyGridView::deleteItemByIndex(int index)
{
	//move slot to previous a slot
	for (int i = index; i < listItems.size() - 1; i++)
	{
		this->listItemSlot[i].setItem(ItemsConfig::getInstance()->getItem(listItems[i + 1].x, listItems[i + 1].y));
	}
	//un-select slot
	this->listItemSlot[index].setSelected(false);
	//clear finnal slot
	this->listItemSlot[listItems.size() - 1].setEmpty();
	//delete item from list
	this->listItems.erase(this->listItems.begin() + index);
	//change to un-select state if pack is empty or delete the end of list
	if (listItems.size() == 0 || index >= listItems.size())
		this->isHaveSelected = false;
}

Slot MyGridView::getSlotByIndex(int index)
{
	if (index < this->listItems.size())
		return this->listItemSlot[index];
}

void MyGridView::setItemSlotByIndex(int index, int idType, int idItem)
{
	this->getSlotByIndex(index).setItem(ItemsConfig::getInstance()->getItem(idType, idItem));
	this->listItems[index].x = idType;
	this->listItems[index].y = idItem;
}

void MyGridView::setSelected(bool isSelected)
{
	for (int i = 0; i < this->totalAvailableSlot; i++)
	{
		this->listItemSlot[i].setSelected(isSelected);
	}
}

void MyGridView::notifyDataChanged()
{
	//create slot
	this->listItemSlot = new Slot[this->totalSlot];

	for (int i = 0; i < totalSlot; i++)
	{
		this->listItemSlot[i].setType(this->typeGrid);
	}
	//set item for slot
	for (int i = 0; i < listItems.size(); i++)
	{
		this->listItemSlot[i].setItem(ItemsConfig::getInstance()->getItem(listItems[i].x, listItems[i].y));
	}
	//calculate row
	row = this->totalSlot%column != 0 ? (int)this->totalSlot / column + 1 : (int)this->totalSlot / column;
	//create matrix position
	this->gridPositon = new Vec2*[row];
	for (int i = 0; i < row; i++)
	{
		gridPositon[i] = new Vec2[this->column];
	}

	Size slotSize;
	if (this->typeGrid == GRID_ITEM)
	{
		slotSize.width = 64;
		slotSize.height = 64;
	}
	else
	{
		slotSize.width = 192;
		slotSize.height = 64;
	}

	//set content size for grid layer and set position
	this->gridLayer->setContentSize(Size(slotSize.width * this->column + verticalSpacing * (this->column - 1), slotSize.height * row + horizontalSpacing * (row - 1)));
	this->gridLayer->setPosition(Vec2(scrollContainer->getContentSize().width / 2 - gridLayer->getContentSize().width / 2, scrollContainer->getContentSize().height - gridLayer->getContentSize().height > 0 ? scrollContainer->getContentSize().height - gridLayer->getContentSize().height : 0));
	//set inner size for scroll
	scrollContainer->setInnerContainerSize(gridLayer->getContentSize());

	//create select cursor item
	Sprite* s = Sprite::create("UI/button/slot-selected.png");
	s->setVisible(false);
	this->gridLayer->addChild(s, 3);
	//init position slot matrix and init slot

	int numberSlotCreated = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < this->column; j++)
		{
			//calculate position
			gridPositon[i][j].x = slotSize.width / 2 + ((this->column + j) % this->column)*(slotSize.width + verticalSpacing);
			gridPositon[i][j].y = slotSize.height / 2 + (row - 1 - (row + i) % (row))*(slotSize.height + horizontalSpacing);
			numberSlotCreated++;
			//create equal total slot
			if (numberSlotCreated <= totalSlot)
			{
				this->listItemSlot[numberSlotCreated - 1].addToParent(this->gridLayer, 1, gridPositon[i][j], s, i * 10 + j, [&](int id, bool isSelected){setSelectedSlot(id, isSelected); });
				//disable slot non-available
				if (numberSlotCreated > totalAvailableSlot)
				{
					this->listItemSlot[numberSlotCreated - 1].setEnable(false);
				}
			}
		}
	}
}