#include "Slot.h"
#include "Import\AudioConfig.h"
Slot::Slot()
{
	slotButton = ui::Button::create("UI/button/slot.png", "UI/button/slot-selected.png", "UI/button/slot-disable.png");
	item = NULL;
	itemImage = NULL;
	m_selected = NULL;
	m_isSelected = false;
	this->slotLayer = Layer::create();
}

Slot::Slot(GameItem* item)
{
	this->item = item;
	this->slotLayer = Layer::create();
}

Slot::Slot(int type)
{
	slotButton = ui::Button::create("UI/button/slot.png", "UI/button/slot-selected.png", "UI/button/slot-disable.png");
	item = NULL;
	itemImage = NULL;
	m_selected = NULL;
	m_isSelected = false;
	this->typeSlot = type;
	this->slotLayer = Layer::create();
}

void Slot::createSlot()
{

}
void Slot::addToParent(Layer* parent, int zOrder, Vec2 position, Sprite* selected, int tag, std::function<void(int, bool)> callback)
{

	switch (typeSlot)
	{
		case SLOT_PRICE:
		{
			this->slotLayer->setContentSize(Size(192, 64));
			this->slotLayer->setPosition(Vec2(position.x - 96, position.y - 32));
			break;
		}
		default:
		{
			this->slotLayer->setContentSize(Size(64, 64));
			this->slotLayer->setPosition(Vec2(position.x - 32, position.y - 32));
			break;
		}
	}
	this->slotButton->setPosition(Vec2(32, 32));
	this->slotLayer->addChild(this->slotButton);

	//this->slotButton->setPosition(position);
	this->slotButton->setTag(tag);
	//parent->addChild(this->slotButton, zOrder);
	parent->addChild(this->slotLayer, zOrder);
	if (item != NULL && itemImage == NULL)
	{
		updateDataSlot();
	}
	this->slotButton->addClickEventListener([&, selected, callback](Ref* event){
		if (this->item != NULL)
		{
			if (m_isSelected)
			{
				AudioConfig::getInstance()->playEffect("UNCHOOSESLOT");
				m_isSelected = false;
				selected->setVisible(false);
				callback(this->slotButton->getTag(), false);
			}
			else
			{
				AudioConfig::getInstance()->playEffect("CHOOSESLOT");
				selected->retain();
				if (selected->getParent() != NULL)
					selected->removeFromParent();
				this->slotButton->addChild(selected, 0);
				selected->setVisible(true);
				selected->setPosition(this->slotButton->getContentSize() / 2);
				m_selected = selected;
				selected->release();
				m_isSelected = true;
				log("%d", this->slotButton->getTag());
				callback(this->slotButton->getTag(), true);
			}
		}
	});

}


void Slot::setItem(GameItem* item)
{
	this->item = item;
	log("set new item %d", item->getIdItem());
	updateDataSlot();
}

void Slot::setEnable(bool isEnable)
{
	this->slotButton->setEnabled(isEnable);
}

void Slot::updateDataSlot()
{
	if (itemImage != NULL)
	{
		itemImage->removeFromParentAndCleanup(true);
	}
	itemImage = Sprite::create(item->getSrc());
	if (itemImage == NULL)
		log("error create image");
	itemImage->setPosition(this->slotButton->getContentSize() / 2);
	itemImage->setScale(this->slotButton->boundingBox().size.width * 9 / (10 * itemImage->getContentSize().width), this->slotButton->boundingBox().size.height * 9 / (10 * itemImage->getContentSize().height));
	if (this->typeSlot == SLOT_PRICE)
	{
		std::string nameItem = this->item->getNameItem();
		if (nameItem.length() > 15)
		{
			nameItem = nameItem.substr(0, 11) + "...";
		}
			//CCString::createWithFormat("%s", this->item->getNameItem().c_str())->getCString()
		Label* itemName = Label::createWithTTF(nameItem, "UI/font/SVN-Prima.ttf", 20);
		itemName->setAnchorPoint(Vec2(0, 0.5));
		itemName->setPosition(Vec2(80, 48));
		this->slotLayer->addChild(itemName);
		Label* itemPrice = Label::createWithTTF(CCString::createWithFormat("%d GOLD", this->item->getPrice())->getCString(), "UI/font/SVN-Prima.ttf", 20);
		itemPrice->setAnchorPoint(Vec2(0, 0.5));
		itemPrice->setColor(Color3B(255, 83, 0));
		itemPrice->setPosition(Vec2(80, 16));
		this->slotLayer->addChild(itemPrice);
	}
	this->slotButton->addChild(itemImage, 1);
}

void Slot::setSelected(bool isSelected)
{
	this->m_isSelected = isSelected;
	if (m_isSelected == false)
	{
		//this->slotButton->removeAllChildrenWithCleanup(true);
		if(this->slotButton->getChildByName("selected") != NULL)
			this->slotButton->getChildByName("selected")->setVisible(false);
	}
}

void Slot::setEmpty()
{
	if (itemImage != NULL && item != NULL)
	{
		itemImage->removeFromParentAndCleanup(true);
		itemImage = NULL;
		this->item = NULL;
	}
}

GameItem* Slot::getItem()
{
	if (this->item == NULL)
		return NULL;
	return this->item;
}

void Slot::setOpacity(int opacity)
{
	this->slotButton->setOpacity(opacity);
}

void Slot::setType(int typeSlot)
{
	this->typeSlot = typeSlot;
}

void Slot::setChoose(bool isChoose)
{
	static Color3B oldColor = this->itemImage->getColor();
	this->itemImage->stopAllActions();
	if (isChoose)
	{
		this->m_isAvailable = true;
		this->itemImage->runAction(TintTo::create(0.001, oldColor.r, oldColor.g, oldColor.b));	
	}
	else
	{
		this->m_isAvailable = false;
		this->itemImage->runAction(TintTo::create(0.001, 100, 100, 100));
	}
}

Vec2 Slot::getPosition()
{
	return this->slotButton->getPosition();
}

bool Slot::isAvailable()
{ 
	return this->m_isAvailable;
}

bool Slot::isSelected()
{
	return this->m_isSelected;
}