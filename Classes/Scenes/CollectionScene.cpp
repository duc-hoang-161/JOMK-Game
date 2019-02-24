#include "Scenes\CollectionScene.h"

Scene* CollectionScene::createScene()
{
	return CollectionScene::create();
}

bool CollectionScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->addChild(this->mainLayer, 0);
	setBackground("UI/button/background-3.png");
	this->setTitleHeader("YOUR COLLECTION");
	this->setIconUnit("UI/button/gold.png");
	LayerColor* splitLayer = LayerColor::create(Color4B(4, 7, 16, 255));
	splitLayer->setContentSize(Size(10, this->contentBackground->getContentSize().height*113/125));
	splitLayer->setPosition(Vec2(this->contentBackground->getContentSize().width * 2 / 3, this->contentBackground->getContentSize().height * 6 / 125));
	this->contentBackground->addChild(splitLayer);
	this->setNumberValue(PackConfig::getInstance()->getCoins());
	PackConfig::getInstance()->sortPack();
	//create layer contain gridview
	Layer* gridContent = Layer::create();
	gridContent->setContentSize(Size(contentLayer->getContentSize().width * 2 / 3, contentLayer->getContentSize().height));
	contentLayer->addChild(gridContent);
	//create grid view
	gridView = new MyGridView();
	gridView->setData(PackConfig::getInstance()->getItemsPack());
	gridView->setCallBackOnItemSelected([&](Vec2 id, bool isSelected){
		onItemSelected(id, isSelected);
	});
	gridView->setParent(gridContent);
	gridView->setTotalSlot(PackConfig::getInstance()->getTotalSlot());
	gridView->setAvailableSlot(PackConfig::getInstance()->getAvailableSlot());
	gridView->setColumn(6);
	gridView->notifyDataChanged();
	createPersonalLayout();
	setActionButton();
	createAlertLayer();
	//init invisible for layer
	this->detailBackground->setVisible(false);
	this->detailLabelTitle->setVisible(false);
	return true;
}


void CollectionScene::setActionButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//FIRST BUTTON
	this->setActionFirstButton("SHOP", [](){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->replaceScene(ShopScene::createScene()); 
	});
	//SECOND BUTTON
	this->setActionSecButton("SELL", [&, this](){
		AudioConfig::getInstance()->playEffect("UI1");
		this->intentId = 1;
		this->alert->setVisible(true);
		this->alert->setTitleAlert(CCString::createWithFormat("DO YOU WANT TO SELL THIS ITEM?\nTHIS ITEM HAS %d GOLD", this->gridView->getItemSelected()->getPrice())->getCString());
		this->setEnable(false);
	});
	this->secActionButton->getButton()->setScale(0.8);
	this->secActionButton->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 0.9 / 11));
	this->secActionButton->setEnabled(false);
	this->secActionButton->setVisible(false);
	//THIRD BUTTON
	this->setActionThirdButton("EXPAND", [&, this](){
		AudioConfig::getInstance()->playEffect("UI1");
		this->alert->setVisible(true);
		int price = PackConfig::getInstance()->getPriceUpgrade();
		int coins = PackConfig::getInstance()->getCoins();
		int slotRemain = PackConfig::getInstance()->getTotalSlot() - PackConfig::getInstance()->getAvailableSlot();
		if (coins >= price && slotRemain > 0)
		{
			this->alert->setTitleAlert(CCString::createWithFormat("DO YOU WANT TO EXPAND PACK?\n THIS ACTION WILL TAKE %d GOLD", PackConfig::getInstance()->getPriceUpgrade())->getCString());
			this->intentId = 2;
		}
		else
		{
			if (slotRemain == 0)
			{
				this->alert->setTitleAlert("MAXIMMUN OF UPGRADE, \nYOU CAN NOT EXPAND MORE");
				this->intentId = 21;
			}
			if (coins < price)
			{
				this->alert->setTitleAlert(CCString::createWithFormat("YOU NOT ENOUGH COIN TO DO \nTHIS ACTION, YOU NEED %d COIN", price)->getCString());
				this->intentId = 20;
			}
		}
		this->setEnable(false);
	});
	this->thirdActionButton->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 7.5 / 12));
}

void CollectionScene::createPersonalLayout()
{
	//create left side layer
	Layer* leftSide = Layer::create();
	leftSide->setContentSize(Size(contentLayer->getContentSize().width / 3, contentLayer->getContentSize().height));
	leftSide->setPosition(Vec2(contentLayer->getContentSize().width *2 / 3, 0));
	contentLayer->addChild(leftSide);
	//create person image
	Sprite* person = Sprite::create("UI/person.png");
	person->setScale(1.3);
	person->setPosition(Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height*2.5/5));
	leftSide->addChild(person);
	//create selected image
	Sprite* selected = Sprite::create("UI/button/slot-selected2.png");
	selected->setVisible(false);
	selected->setOpacity(90);
	leftSide->addChild(selected);
	//create slot for main character equip
	slotBody = new Slot[7];
	for (int i = 0; i < 5; i++)
	{
		slotBody[i].setOpacity(90);
	}
	//head
	slotBody[0].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 3.6 / 5), selected, 0, [&](int id, bool isSelected){onSlotBodySelected(0, isSelected); });
	//left screen
	slotBody[1].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *0.4 / 2, leftSide->getContentSize().height * 2.25 / 5), selected, 0, [&](int id, bool isSelected){onSlotBodySelected(1, isSelected); });
	//right screen
	slotBody[2].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *1.6 / 2, leftSide->getContentSize().height * 2.25 / 5), selected, 0, [&](int id, bool isSelected){onSlotBodySelected(2, isSelected); });
	//body
	slotBody[3].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 2.5 / 5), selected, 0, [&](int id, bool isSelected){onSlotBodySelected(3, isSelected); });
	//foot
	slotBody[4].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 1.1 / 5), selected, 0, [&](int id, bool isSelected){onSlotBodySelected(4, isSelected); });
	//skill 1 left screen
	slotBody[5].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *0.4 / 2, leftSide->getContentSize().height * 0.5 / 5), selected, 0, [&](int id, bool isSelected){});
	slotBody[5].setEnable(false);
	//skill 2 right screen
	slotBody[6].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *1.6 / 2, leftSide->getContentSize().height * 0.5 / 5), selected, 0, [&](int id, bool isSelected){});
	slotBody[6].setEnable(false);
	//create button equip
	btnEquip = new Button(Button::LIGHT_BROWN, "EQUIP", Color3B(96, 57, 19));
	btnEquip->setPosition(Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 4.6 / 5));
	leftSide->addChild(btnEquip->getButton());
	btnEquip->setEnabled(false);
	this->btnEquip->getButton()->setOpacityModifyRGB(false);
	this->btnEquip->getButton()->setOpacity(100);

	for (int i = 0; i < 7; i++)
	{
		if (PackConfig::getInstance()->getEquipItemAt(i) != NULL)
			slotBody[i].setItem(PackConfig::getInstance()->getEquipItemAt(i));
		else
			log("not equip %d", i);
	}

	btnEquip->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("EQUIP");
		if (this->btnEquip->getButton()->getTitleText() == "EQUIP")
		{
			log("equip");
			if (gridView->getItemSelected()->getTypeItem() == 333)
			{
				EquipItem* item = static_cast<EquipItem*>(this->gridView->getItemSelected());
				int position = item->getPositionEquip();

				this->slotBody[position].setItem(gridView->getItemSelected());
				PackConfig::getInstance()->setEquipItemAt(position, gridView->getIndexItemSelected(), Vec2(gridView->getItemSelected()->getTypeItem(), gridView->getItemSelected()->getIdItem()));
			}
			if (gridView->getItemSelected()->getTypeItem() == 222)
			{
				this->slotBody[1].setItem(gridView->getItemSelected());
				PackConfig::getInstance()->setEquipItemAt(1, gridView->getIndexItemSelected(), Vec2(gridView->getItemSelected()->getTypeItem(), gridView->getItemSelected()->getIdItem()));
			}
		}
		else
		{
			log("unequip");
			this->slotBody[this->btnEquip->getButton()->getTag()].setEmpty();
			PackConfig::getInstance()->setEquipItemAt(this->btnEquip->getButton()->getTag(), -1, Vec2(-1, -1));
			this->btnEquip->setTitleText("EQUIP");
			//this->btnEquip->setEnabled(false);
			//this->btnEquip->getButton()->setOpacity(100);
		}
	});
}

void CollectionScene::onItemSelected(Vec2 id, bool isSelected)
{
	if (isSelected)
	{
		if (this->btnEquip->getButton()->getTitleText() == "UNEQUIP")
		{
			this->btnEquip->setTitleText("EQUIP");
			/*this->btnEquip->setEnabled(false);
			this->btnEquip->getButton()->setOpacityModifyRGB(false);
			this->btnEquip->getButton()->setOpacity(100);*/
		}
		setDisplayDetail(gridView->getItemSelected()->showInfo(), isSelected);
		if (gridView->getItemSelected()->getTypeItem() == 333 || gridView->getItemSelected()->getTypeItem() == 222)
		{
			this->btnEquip->setEnabled(true);
			this->btnEquip->getButton()->setOpacityModifyRGB(false);
			this->btnEquip->getButton()->setOpacity(-1);
			
			log("++++++%d", gridView->getIndexItemSelected());
		}
		else
		{
			this->btnEquip->setEnabled(false);
			this->btnEquip->getButton()->setOpacityModifyRGB(false);
			this->btnEquip->getButton()->setOpacity(100);
		}
		for (int i = 0; i < 6; i++)
		{
			slotBody[i].setSelected(false);
		}
	}
	else
	{
		this->btnEquip->setEnabled(false);
		this->btnEquip->getButton()->setOpacityModifyRGB(false);
		this->btnEquip->getButton()->setOpacity(100);
		setDisplayDetail("", isSelected);
	}
}

void CollectionScene::setDisplayDetail(std::string text, bool isVisible)
{
	this->detailText->setString(text);
	this->scrollDetail->setInnerContainerSize(Size(this->scrollDetail->getInnerContainerSize().width, this->detailText->getContentSize().height));
	this->secActionButton->setEnabled(isVisible);
	this->secActionButton->setVisible(isVisible);
	this->detailBackground->setVisible(isVisible);
	this->detailLabelTitle->setVisible(isVisible);
	
}

void CollectionScene::confirmSell()
{

}

void CollectionScene::receiver(bool value)
{
	if (value)
	{
		switch (this->intentId)
		{
				//WHEN CLICK OK ON SELL ITEM
			case 1:
			{
				if (gridView->getItemSelected() != NULL)
				{
					PackConfig::getInstance()->addCoins(gridView->getItemSelected()->getPrice());
					int returnValue = PackConfig::getInstance()->removeItemFromCollection(gridView->getIndexItemSelected(),
						Vec2(gridView->getItemSelected()->getTypeItem(), gridView->getItemSelected()->getIdItem()));
					if (returnValue != -1)
					{
						this->slotBody[returnValue].setEmpty();
					}
					gridView->deleteSelectedItem();
					int indexInpack = gridView->getIndexItemSelected();
					for (int i = 0; i < 5; i++)
					{
						PackConfig::getInstance()->getEquipItemAt(i);
					}
					this->setNumberValue(PackConfig::getInstance()->getCoins());
					setDisplayDetail("", false);
					if (this->btnEquip->isEnabled())
					{
						this->btnEquip->setEnabled(false);
						this->btnEquip->getButton()->setOpacityModifyRGB(false);
						this->btnEquip->getButton()->setOpacity(100);
					}
				}
				this->intentId = 0;
				break;
			}
				//WHEN CLICK OK ON EXPAND COLLECTION SIZE
			case 2:
			{
				int price = PackConfig::getInstance()->getPriceUpgrade();
				if (PackConfig::getInstance()->getCoins() >= price)
				{
					PackConfig::getInstance()->subCoins(price);
					this->setNumberValue(PackConfig::getInstance()->getCoins());
					gridView->expandSlot(PackConfig::getInstance()->getExpandStep());
					PackConfig::getInstance()->expandPackSize();
					this->intentId = 0;
				}
				break;
				  
			}
				//WHEN CLICK OK ON ALERT NOT ENOUGH COIN TO EXPAND
			case 20:
			{
				//could take player to pay for more coin
				this->intentId = 0;
				break;
			}
				//WHEN CLICK OK ON ALERT MAXIMUM UPGRADE
			case 21:
			{
				this->intentId = 0;
				break;
			}
			default:
			{
				this->intentId = 0;
				break;
			}
		}
		//invisible after click OK
		if (this->alert->isVisible() && this->intentId == 0)
			this->alert->setVisible(false);
	}
	else
	{
		//invisible when click cancel
		this->alert->setVisible(false);
		this->intentId = 0;
	}
	//enable background button
	if (this->intentId == 0)
		this->setEnable(true);
}

void CollectionScene::setEnable(bool isEnable)
{
	this->firstActionButton->setEnabled(isEnable);
	this->secActionButton->setEnabled(isEnable);
	this->thirdActionButton->setEnabled(isEnable);
	this->btnBack->setEnabled(isEnable);
	this->gridView->setEnable(isEnable);
	this->btnEquip->setVisible(isEnable);
}

void CollectionScene::createAlertLayer()
{
	alert = new Alert("DO YOU WANT SELL THIS ITEM?", "", Alert::CONFIRM_SIZE);
	alert->setVisible(false);
	this->addChild(alert->getAlertLayer(), 8);
	alert->getRightActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(false);
	});
	alert->getLeftActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(true);
	});
}

void CollectionScene::onSlotBodySelected(int index, bool isSelected)
{
	if (isSelected)
	{
		for (int i = 0; i < 6; i++)
		{
			if (i != index)
				this->slotBody[i].setSelected(false);
		}
		if (slotBody[index].getItem() != NULL)
		{
			this->btnEquip->setEnabled(true);
			this->btnEquip->getButton()->setOpacityModifyRGB(false);
			this->btnEquip->getButton()->setOpacity(-1);
			this->btnEquip->setTitleText("UNEQUIP");
			this->btnEquip->getButton()->setTag(index);
		}
	}
	else
	{
		this->btnEquip->setTitleText("EQUIP");
		this->btnEquip->setEnabled(false);
		this->btnEquip->getButton()->setOpacityModifyRGB(false);
		this->btnEquip->getButton()->setOpacity(100);
	}
}