#include "Scenes\ShopScene.h"

Scene* ShopScene::createScene()
{
	return ShopScene::create();
}

bool ShopScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//init layout
	this->addChild(this->mainLayer, 0);
	setBackground("UI/button/background-3.png");
	this->setTitleHeader("SHOP");
	this->setIconUnit("UI/button/gold.png");
	this->setNumberValue(PackConfig::getInstance()->getCoins());
	this->detailBackground->setVisible(false);
	this->scrollDetail->setContentSize(Size(scrollDetail->getContentSize().width, detailBackground->getContentSize().height * 8 / 10));
	this->scrollDetail->setPosition(Vec2(0, 10));
	
	createLayout();
	//create grid view
	Layer* gridLayer = Layer::create();
	gridLayer->setContentSize(Size(this->contentLayer->getContentSize().width, this->contentLayer->getContentSize().height - this->btnWeapon->getButton()->getContentSize().height - 5));
	this->contentLayer->addChild(gridLayer);
	gridView = new MyGridView(MyGridView::GRID_PRICE);
	gridView->setData(ItemsConfig::getInstance()->getBuyItem(111));
	gridView->setCallBackOnItemSelected([&](Vec2 id, bool isSelected){
		onItemSelected(id, isSelected);
	});
	gridView->setSpaceVertical(64);
	gridView->setSpaceHorizontal(32);
	gridView->setParent(gridLayer);
	gridView->setTotalSlot(ItemsConfig::getInstance()->getBuyItem(111).size());
	gridView->setAvailableSlot(ItemsConfig::getInstance()->getBuyItem(111).size());
	gridView->setColumn(3);
	gridView->notifyDataChanged();
	this->typeItemShop = 1;
	createAlertLayer();
	setActionButton();

	return true;
}

void ShopScene::createLayout()
{
	Size sizeContent = this->contentLayer->getContentSize();
	btnSupport = new Button(Button::LIGHT_BROWN, "SUPPORT ITEM");
	color = this->btnSupport->getButton()->getColor();
	
	btnSupport->setPosition(Vec2(sizeContent.width / 4 - 10, sizeContent.height - btnSupport->getButton()->getContentSize().height/2));
	this->contentLayer->addChild(btnSupport->getButton(), 2);
	btnSupport->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		if (this->typeItemShop != 1)
		{
			this->btnSupport->getButton()->runAction(TintTo::create(0.01, this->color.r, this->color.g, this->color.b)); 
			this->btnEquip->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->btnWeapon->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->typeItemShop = 1;
			gridView->setData(ItemsConfig::getInstance()->getBuyItem(111));
			gridView->setTotalSlot(ItemsConfig::getInstance()->getBuyItem(111).size());
			gridView->setAvailableSlot(ItemsConfig::getInstance()->getBuyItem(111).size());
			gridView->setColumn(3);
			gridView->notifyDataChanged();
		}
	});

	btnWeapon = new Button(Button::LIGHT_BROWN, "WEAPON ITEM");
	btnWeapon->setPosition(Vec2(sizeContent.width / 2, sizeContent.height - btnSupport->getButton()->getContentSize().height / 2));
	this->contentLayer->addChild(btnWeapon->getButton(), 2);
	btnWeapon->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		if (this->typeItemShop != 2)
		{
			this->btnWeapon->getButton()->runAction(TintTo::create(0.01, this->color.r, this->color.g, this->color.b)); 
			this->btnEquip->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->btnSupport->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->typeItemShop = 2;
			gridView->setData(ItemsConfig::getInstance()->getBuyItem(222));
			gridView->setTotalSlot(ItemsConfig::getInstance()->getBuyItem(222).size());
			gridView->setAvailableSlot(ItemsConfig::getInstance()->getBuyItem(222).size());
			gridView->setColumn(3);
			gridView->notifyDataChanged();
		}
	});
	btnEquip = new Button(Button::LIGHT_BROWN, "EQUIP ITEM");
	btnEquip->setPosition(Vec2(sizeContent.width * 3 / 4 + 10, sizeContent.height - btnSupport->getButton()->getContentSize().height / 2));
	this->contentLayer->addChild(btnEquip->getButton(), 2);
	btnEquip->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		if (this->typeItemShop != 3)
		{
			this->btnEquip->getButton()->runAction(TintTo::create(0.01, this->color.r, this->color.g, this->color.b)); 
			this->btnWeapon->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->btnSupport->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
			this->typeItemShop = 3;
			gridView->setData(ItemsConfig::getInstance()->getBuyItem(333));
			gridView->setTotalSlot(ItemsConfig::getInstance()->getBuyItem(333).size());
			gridView->setAvailableSlot(ItemsConfig::getInstance()->getBuyItem(333).size());
			gridView->setColumn(3);
			gridView->notifyDataChanged();
		}
	});
	//this->btnSupport->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
	this->btnWeapon->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
	this->btnEquip->getButton()->runAction(TintTo::create(0.001, 100, 100, 100));
}

void ShopScene::setActionButton()
{
	//FIRST BUTTON
	this->firstActionButton->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->replaceScene(CollectionScene::createScene());
	});
	this->firstActionButton->setTitleText("Collection");
	//SECOND BUTTON
	this->secActionButton->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		if (this->gridView->getItemSelected() != NULL)
		{
			if (PackConfig::getInstance()->getAvailableSlot() > PackConfig::getInstance()->getItemsPack().size())
			{
				if (PackConfig::getInstance()->getCoins() >= this->gridView->getItemSelected()->getPrice())
				{
					PackConfig::getInstance()->subCoins(gridView->getItemSelected()->getPrice());
					PackConfig::getInstance()->addItem(Vec2(this->gridView->getItemSelected()->getTypeItem(), gridView->getItemSelected()->getIdItem()));
					this->setNumberValue(PackConfig::getInstance()->getCoins());
					toastNewItem("+ 1 " + this->gridView->getItemSelected()->getNameItem());
				}
				else
				{
					alert->setTitleAlert("SORRY, YOU NOT ENOUGH COINS \nTO BUY THIS ITEM");
					alert->setVisible(true);
					alert->getLeftActionButton()->setTitleText("REFILL");
					this->setEnable(false);
					this->intentId = 2;
				}
			}
			else
			{
				alert->setTitleAlert("YOUR PACK NOT ENOUGH SPACE, \nYOU NEED UPGRADE YOUR COLLECTION SPACE");
				alert->getTitleLabel()->setScale(0.8);
				alert->setVisible(true);
				alert->getLeftActionButton()->setTitleText("EXPAND");
				this->setEnable(false);
				this->intentId = 1;
			}
		}
	});
	this->secActionButton->setTitleText("BUY");
	this->secActionButton->setPosition(this->thirdActionButton->getPosition());
	this->secActionButton->setVisible(false);
	//THIRD BUTTON
	this->thirdActionButton->setVisible(false);
}

void ShopScene::toastNewItem(std::string mes)
{
	this->toastBuyItem->stopAllActions();
	this->toastBuyItem->setVisible(true);
	this->toastBuyItem->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->toastBuyItem->setOpacity(255);
	this->toastBuyItem->setString(mes);
	this->toastBuyItem->runAction(Spawn::createWithTwoActions(Repeat::create(MoveBy::create(0.5, Vec2(0, 150)), 1), Repeat::create(FadeOut::create(0.5), 1)));
}

void ShopScene::onItemSelected(Vec2 id, bool isSelected)
{
	this->secActionButton->setVisible(isSelected);
	this->detailBackground->setVisible(isSelected);
	if (isSelected)
	{
		this->detailText->setString(gridView->getItemSelected()->showInfo());
		this->scrollDetail->setInnerContainerSize(Size(this->scrollDetail->getInnerContainerSize().width, this->detailText->getContentSize().height));
	}
}

void ShopScene::receiver(bool value)
{
	if (value)
	{
		switch (this->intentId)
		{
			//WHEN NOT ENOUGH SPACE ON COLLECTION
			case 1:
			{
					  //expand button redirect to collection scene
				Director::getInstance()->replaceScene(CollectionScene::createScene());
				break;
			}
			//WHEN NOT ENOUGH COIN, REFILL BUTTON
			case 2:
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
		//invisible after click left button
		if (this->alert->getAlertLayer()->isVisible() && this->intentId == 0)
			this->alert->setVisible(false);
	}
	else
	{
		//invisible after click right button
		this->alert->setVisible(false);
		this->intentId = 0;
		this->alert->getRightActionButton()->setTitleText("CANCEL");
		this->alert->getLeftActionButton()->setTitleText("OK");
	}
	//enable background button after invisible alert
	if (this->intentId == 0)
		this->setEnable(true);
	this->alert->getTitleLabel()->setScale(1);
}

void ShopScene::setEnable(bool isEnable)
{
	this->firstActionButton->setEnabled(isEnable);
	this->gridView->setEnable(isEnable);
	this->btnBack->setEnabled(isEnable);
	this->btnSupport->setEnabled(isEnable);
	this->btnWeapon->setEnabled(isEnable);
	this->btnEquip->setEnabled(isEnable);
}

void ShopScene::createAlertLayer()
{
	//create toast text
	this->toastBuyItem = Label::createWithTTF("+ NEW ITEM", "UI/font/000 Meanwhile.ttf", 40);
	this->toastBuyItem->setTextColor(Color4B::YELLOW);
	this->toastBuyItem->setVisible(false);
	this->toastBuyItem->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(this->toastBuyItem, 5);
	//create alert
	this->alert = new Alert("THIS IS ALERT MESSAGE", "", Alert::CONFIRM_SIZE);
	this->alert->setVisible(false);
	this->alert->getRightActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(false);
	});
	this->alert->getLeftActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(true);
	});
	this->addChild(this->alert->getAlertLayer(), 5);
}