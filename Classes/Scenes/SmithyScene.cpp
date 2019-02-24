#include "Scenes\SmithyScene.h"

Scene* SmithyScene::createScene()
{
	return SmithyScene::create();
}

bool SmithyScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	this->addChild(this->mainLayer, 0);
	setBackground("UI/button/background-4.png");
	this->setTitleHeader("SMITHY");
	this->setNumberValue(PackConfig::getInstance()->getCoins());
	this->setIconUnit("UI/button/gold.png");
	btnBack->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		PackConfig::getInstance()->sortPack();
		Director::getInstance()->popScene();
	});
	createGridView();

	createLayout();
	return true;
}

void SmithyScene::createLayout()
{
	this->firstActionButton->setTitleText("Upgrade");
	this->firstActionButton->setVisible(false);
	this->firstActionButton->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("SELECTUPGRADE");
		this->buildNew->stopAllActions();
		this->buildNew->setVisible(true);
		auto nextUpgrade = CallFunc::create([&, this](){ this->buildNew->setVisible(false); this->upgradeItem(); });
		auto sequence = Sequence::createWithTwoActions(Repeat::create(Animate::create(Animation::createWithSpriteFrames(this->animationBuild, 0.5f/6)),1), nextUpgrade);
		this->buildNew->runAction(sequence);
		
	});
	this->secActionButton->setVisible(false);
	this->thirdActionButton->setVisible(false);

	this->detailBackground->setVisible(false);
	this->intentId = 0;
	this->alert = new Alert("TITLE", "", Alert::CONFIRM_SIZE);
	this->alert->setVisible(false);
	this->alert->getLeftActionButton()->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(true);
	});
	this->alert->getRightActionButton()->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(false);
	});
	this->addChild(this->alert->getAlertLayer(), 6);

	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("res/Plist2.plist");
	
	for (int i = 1; i <= 6; i++)
	{
		this->animationBuild.pushBack(spriteCache->getSpriteFrameByName(CCString::createWithFormat("p%d.png", i)->getCString()));
	}
	buildNew = Sprite::createWithSpriteFrame(animationBuild.front());
	buildNew->setPosition(this->costUpgrade->getPosition());
	buildNew->setVisible(false);
	this->contentLayer->addChild(buildNew, 5);


}

void SmithyScene::createGridView()
{
	//create container for grid piece
	Size containerSize = this->contentLayer->getContentSize();
	Layer* layerGridPiece = LayerColor::create();
	layerGridPiece->setContentSize(Size(containerSize.width/3, containerSize.height/2 - 5));
	layerGridPiece->setPosition(Vec2(containerSize.width * 2 / 3, containerSize.height / 2 + 5));
	this->contentLayer->addChild(layerGridPiece);
	//create grid piece
	std::vector<Vec2> listPiece = PackConfig::getInstance()->getListItemById(444);
	this->gridViewPiece = new MyGridView();
	this->gridViewPiece->setData(listPiece);
	this->gridViewPiece->setCallBackOnItemSelected([&, this](Vec2 indexGrid, bool isSelected){
		onRockItemSelected(indexGrid, isSelected);
	});
	this->gridViewPiece->setParent(layerGridPiece);
	this->gridViewPiece->setTotalSlot(listPiece.size());
	this->gridViewPiece->setAvailableSlot(listPiece.size());
	this->gridViewPiece->setColumn(3);
	this->gridViewPiece->notifyDataChanged();

	//create container for grid upgrade item
	Layer* layerGridItem = LayerColor::create();
	layerGridItem->setContentSize(layerGridPiece->getContentSize());
	layerGridItem->setPosition(Vec2(containerSize.width * 2 / 3, 0));
	this->contentLayer->addChild(layerGridItem);
	//create grid item
	listItem = PackConfig::getInstance()->getListUpgradeItem();
	std::vector<Vec2> listItemId;
	for (int i = 0; i < listItem.size(); i++)
	{
		listItemId.push_back(Vec2(listItem[i].y, listItem[i].z));
	}
	this->gridViewUpgradeItems = new MyGridView();
	this->gridViewUpgradeItems->setData(listItemId);
	this->gridViewUpgradeItems->setCallBackOnItemSelected([&](Vec2 indexGrid, bool isSelected){
		onUpgradeItemSelected(indexGrid, isSelected);
	});
	this->gridViewUpgradeItems->setParent(layerGridItem);
	this->gridViewUpgradeItems->setTotalSlot(listItemId.size());
	this->gridViewUpgradeItems->setAvailableSlot(listItemId.size());
	this->gridViewUpgradeItems->setColumn(3);
	this->gridViewUpgradeItems->notifyDataChanged();

	//create upgrade layer
	Layer* upgradeLayer = LayerColor::create();
	upgradeLayer->setContentSize(Size(containerSize.width * 2 / 3, containerSize.height));
	upgradeLayer->setPosition(Vec2::ZERO);
	this->contentLayer->addChild(upgradeLayer);
	Size upgradeSize = upgradeLayer->getContentSize();
	//create slot for upgrade
	Sprite* selected = Sprite::create("UI/button/slot-selected.png");
	selected->setVisible(false);
	upgradeLayer->addChild(selected);
	this->slotUpgrade = new Slot(Slot::SLOT_ITEM);
	this->slotUpgrade->addToParent(upgradeLayer, 1, Vec2(upgradeSize.width/4, upgradeSize.height*4/5), selected, 11, [&, this](int id, bool isSelected){
		if (this->slotUpgrade->getItem() != NULL)
		{
			this->slotUpgrade->setSelected(false);
			this->slotUpgrade->setEmpty();
			if (this->slotNextItem->getItem() != NULL)
			{
				this->slotNextItem->setSelected(false);
				this->slotNextItem->setEmpty();
			}
			this->firstActionButton->setVisible(false);
			this->costUpgrade->setString("-");
			this->percentUpgrade->setString("X");
		}
		
		
	});
	this->slotPiece = new Slot(Slot::SLOT_ITEM);
	this->slotPiece->addToParent(upgradeLayer, 1, Vec2(upgradeSize.width * 3 / 4, upgradeSize.height * 4 / 5), selected, 22, [&, this](int id, bool isSelected){
		if (this->slotPiece->getItem() != NULL)
		{
			this->slotPiece->setEmpty();
			this->firstActionButton->setVisible(false);
			this->costUpgrade->setString("-");
			this->percentUpgrade->setString("X");
		}
	});

	this->slotNextItem = new Slot(Slot::SLOT_ITEM);
	this->slotNextItem->addToParent(upgradeLayer, 1, Vec2(upgradeSize.width / 2, upgradeSize.height / 5), selected, 33, [&, this](int id, bool isSelected){
		if (this->slotNextItem->getItem() != NULL)
		{
			this->showInfo(this->slotNextItem->getItem()->showInfo(), isSelected);
		}
	});

	Layer* c1 = LayerColor::create(Color4B::ORANGE);
	Layer* c2 = LayerColor::create(Color4B::ORANGE);
	Layer* c3 = LayerColor::create(Color4B::ORANGE);
	Layer* c4 = LayerColor::create(Color4B::ORANGE);

	c1->setContentSize(Size(10, upgradeSize.height * 1.5 / 5));
	c1->setPosition(Vec2(upgradeSize.width / 4 - 5, upgradeSize.height * 2.5 / 5));
	upgradeLayer->addChild(c1);

	c2->setContentSize(Size(10, upgradeSize.height * 1.5 / 5));
	c2->setPosition(Vec2(upgradeSize.width * 3 / 4 - 5, upgradeSize.height * 2.5 / 5));
	upgradeLayer->addChild(c2);

	c3->setContentSize(Size(upgradeSize.width / 2 + 10, 10));
	c3->setPosition(Vec2(upgradeSize.width / 4 - 5, upgradeSize.height * 2.5 / 5));
	upgradeLayer->addChild(c3);

	c4->setContentSize(Size(10, upgradeSize.height * 1.5 / 5));
	c4->setPosition(Vec2(upgradeSize.width / 2 - 5, upgradeSize.height * 1 / 5));
	upgradeLayer->addChild(c4);

	Sprite* coinBg = Sprite::create("UI/button/panel-bg.png");
	coinBg->setPosition(Vec2(upgradeSize.width / 2, upgradeSize.height / 2));
	coinBg->setScale(0.6);
	coinBg->runAction(TintTo::create(0.001, Color3B(200, 90, 0)));
	upgradeLayer->addChild(coinBg);

	Sprite* percentBg = Sprite::createWithTexture(coinBg->getTexture());
	percentBg->setPosition(Vec2(upgradeSize.width * 3.25 / 4, upgradeSize.height * 1.5 / 5));
	percentBg->setScaleX(0.6);
	percentBg->runAction(TintTo::create(0.001, Color3B(135, 97, 94)));
	upgradeLayer->addChild(percentBg);

	LayerColor* splitLayer = LayerColor::create(Color4B(4, 7, 16, 255));
	splitLayer->setContentSize(Size(10, this->contentBackground->getContentSize().height * 113 / 125));
	splitLayer->setPosition(Vec2(this->contentBackground->getContentSize().width * 2 / 3 - 10, this->contentBackground->getContentSize().height * 6 / 125));
	this->contentBackground->addChild(splitLayer, 2);
	LayerColor* splitLayer2 = LayerColor::create(Color4B(4, 7, 16, 255));
	splitLayer2->setContentSize(Size(layerGridItem->getContentSize().width, 5));
	splitLayer2->setPosition(Vec2(this->contentBackground->getContentSize().width * 2 / 3 - 4, containerSize.height / 2 + 22));
	this->contentBackground->addChild(splitLayer2, 2);
	
	this->toastText = Label::createWithTTF("+ NEW ITEM", "UI/font/000 Meanwhile.ttf", 40);
	this->toastText->setTextColor(Color4B::YELLOW);
	this->toastText->setVisible(false);
	this->toastText->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(this->toastText, 5);

	this->costUpgrade = Label::createWithTTF("-", "UI/font/SVN-Prima.ttf", 30);
	this->costUpgrade->setTextColor(Color4B::YELLOW);
	this->costUpgrade->setPosition(coinBg->getPosition());
	upgradeLayer->addChild(this->costUpgrade, 5);

	this->percentUpgrade = Label::createWithTTF("X", "UI/font/UTM Karate.ttf", 40);
	this->percentUpgrade->setTextColor(Color4B::YELLOW);
	this->percentUpgrade->setPosition(percentBg->getPosition());
	upgradeLayer->addChild(this->percentUpgrade, 5);
}

void SmithyScene::onRockItemSelected(Vec2 indexGrid, bool isSelected)
{
	if (isSelected)
	{
		if (this->slotUpgrade->getItem() != NULL)
		{
			PieceItem* rockSelected = static_cast<PieceItem*>(gridViewPiece->getItemSelected());
			if (this->slotUpgrade->getItem()->getTypeItem() != rockSelected->getKey())
			{
				//toast not suitable to upgrade and return
				this->toastAText("This piece item not suitable on item to upgrade");
				return;
			}
			this->readyToUpgrade();
		}
		this->slotPiece->setItem(gridViewPiece->getItemSelected());
		this->slotPiece->setSelected(true);
		this->indexRock = indexGrid.x * 3 + indexGrid.y;
	}
	else
	{

	}
	if (this->gridViewUpgradeItems->getItemSelected() != NULL)
	{
		this->gridViewUpgradeItems->getSlotByIndex(this->gridViewUpgradeItems->getIndexItemSelected()).setSelected(false);
	}
	if (this->gridViewPiece->getItemSelected() != NULL)
		this->showInfo(this->gridViewPiece->getItemSelected()->showInfo(), isSelected);
}

void SmithyScene::onUpgradeItemSelected(Vec2 indexGrid, bool isSelected)
{
	if (isSelected)
	{
		if (this->slotPiece->getItem() != NULL)
		{
			int typeIdUpgrade = static_cast<PieceItem*>(this->slotPiece->getItem())->getKey();
			if (typeIdUpgrade != gridViewUpgradeItems->getItemSelected()->getTypeItem())
			{
				//toast not suitable to upgrade and return
				this->toastAText("This item not suitable on piece item to upgrade");
				return;
			}
			
		}
		//set index of the item selected
		this->indexUpgrade = indexGrid.x * 3 + indexGrid.y;
		//get selected item to slot upgrade
		int typeId = gridViewUpgradeItems->getItemSelected()->getTypeItem();
		this->slotUpgrade->setItem(gridViewUpgradeItems->getItemSelected());
		this->slotUpgrade->setSelected(true);
		//find next item to upgrade
		int idNext = -1;
		if (typeId == Weapon::ID_WEAPON_ITEM)
		{
			idNext = static_cast<Weapon*>(gridViewUpgradeItems->getItemSelected())->getnextLvlId();
		}
		if (typeId == EquipItem::ID_EQUIP_ITEM)
		{
			idNext = static_cast<EquipItem*>(gridViewUpgradeItems->getItemSelected())->getNextLvl();
		}
		//if item not maximum level
		if (idNext != -1)
		{
			this->slotNextItem->setItem(ItemsConfig::getInstance()->getItem(typeId, idNext));
			if (this->slotPiece->getItem() != NULL)
			{
				this->readyToUpgrade();
			}
		}
		else
		{
			//if item is maximum level
			this->slotNextItem->setEmpty();
			this->toastAText("This item counld not upgrade");
		}
	}
	else
	{

	}
	if (this->gridViewPiece->getItemSelected() != NULL)
	{
		this->gridViewPiece->getSlotByIndex(this->gridViewPiece->getIndexItemSelected()).setSelected(false);
	}
	if (this->gridViewUpgradeItems->getItemSelected() != NULL)
		this->showInfo(this->gridViewUpgradeItems->getItemSelected()->showInfo(), isSelected);
}

void SmithyScene::onSlotUpgradeSelected(Vec2 indexSlot, bool isSelected)
{
	int index = indexSlot.x * 10 + indexSlot.y;
	switch (index)
	{
		case 11:
		{

				  break;
		}
		default:
			break;
	}
}

void SmithyScene::toastAText(std::string mes)
{
	this->toastText->stopAllActions();
	this->toastText->setVisible(true);
	this->toastText->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->toastText->setOpacity(255);
	this->toastText->setString(mes);
	this->toastText->runAction(Spawn::createWithTwoActions(Repeat::create(MoveBy::create(1.5, Vec2(0, 200)), 1), Repeat::create(FadeOut::create(1.5), 1)));
}

bool SmithyScene::getResultUpgrade(int value)
{
	int randInt = RandomHelper::random_int(1, 100);
	if (value > randInt)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SmithyScene::showUpgradeResult(int mode)
{
	static Sprite* success = NULL;
	if (success == NULL)
	{
		success = Sprite::create("UI/upSuccess.png");
		success->setPosition(Director::getInstance()->getVisibleSize() / 2);
		success->setVisible(false);
		this->addChild(success);
	}
	
	switch (mode)
	{
	case 1:
	{
			  success->stopAllActions();
			  success->setPosition(Director::getInstance()->getVisibleSize() / 2);
			  success->setVisible(true);
			  success->setOpacity(255);
			  success->runAction(Spawn::createWithTwoActions(Repeat::create(MoveBy::create(1.5, Vec2(0, 200)), 1), Repeat::create(FadeOut::create(1.5), 1)));
			  break;
	}
	default:
		break;
	}
}

void SmithyScene::readyToUpgrade()
{
	if (this->slotNextItem->getItem() != NULL)
	{
		this->costUpgrade->setString(CCString::createWithFormat("%d", 
			this->slotNextItem->getItem()->getPrice() / 2)->getCString());
		int lvl = this->slotUpgrade->getItem()->getTypeItem() == 222 ?
			static_cast<Weapon*>(this->slotUpgrade->getItem())->getWeaponLvl() : 
			static_cast<EquipItem*>(this->slotUpgrade->getItem())->getCurrentLvl();
		this->percentUpgrade->setString(CCString::createWithFormat("%d%%",
			(int) (static_cast<PieceItem*>(gridViewPiece->getItemSelected())->getValue() / lvl)
			)->getCString()); 
		this->firstActionButton->setVisible(true);
	}
	else
	{
		this->costUpgrade->setString("-");
		this->percentUpgrade->setString("X");
		this->firstActionButton->setVisible(false);
	}
		
}

void SmithyScene::showInfo(std::string info, bool isVisible)
{
	this->detailBackground->setVisible(isVisible);
	this->detailText->setString(info);
	this->scrollDetail->setInnerContainerSize(Size(this->scrollDetail->getInnerContainerSize().width, this->detailText->getContentSize().height));
}

void SmithyScene::upgradeItem()
{
	int cost = -1;
	if (this->slotUpgrade->getItem() != NULL)
	{
		cost = this->slotNextItem->getItem()->getPrice() / 2;
	}
	if (PackConfig::getInstance()->getCoins() >= cost && cost != -1)
	{
		PackConfig::getInstance()->subCoins(cost);
		this->setNumberValue(PackConfig::getInstance()->getCoins());
		// get upgrade item result
		int lvl = this->slotUpgrade->getItem()->getTypeItem() == 222 ?
			static_cast<Weapon*>(this->slotUpgrade->getItem())->getWeaponLvl() :
			static_cast<EquipItem*>(this->slotUpgrade->getItem())->getCurrentLvl();
		int value = static_cast<PieceItem*>(this->slotPiece->getItem())->getValue() / lvl;
		//if upgrade successful
		if (this->getResultUpgrade(value))
		{
			AudioConfig::getInstance()->playEffect("SUCCESS");
			//this->toastAText("Upgrade Successful");
			this->showUpgradeResult(1);
			PackConfig::getInstance()->upgradeItem(this->listItem[this->indexUpgrade].x);
			this->gridViewUpgradeItems->setItemSlotByIndex(
				this->indexUpgrade,
				this->slotNextItem->getItem()->getTypeItem(),
				this->slotNextItem->getItem()->getIdItem()
				);
			//this->gridViewUpgradeItems->
			this->slotNextItem->setEmpty();
			this->slotNextItem->setSelected(false);
			this->slotUpgrade->setEmpty();
			this->slotUpgrade->setSelected(false);
			this->costUpgrade->setString("-");
			this->percentUpgrade->setString("X");
		}
		//if upgrade fail
		else
		{
			this->showUpgradeResult(0);
			this->toastAText("Upgrade fail");
			this->percentUpgrade->setString("X");
		}
		//remove piece item
		PackConfig::getInstance()->removeItem(Vec2(
			this->slotPiece->getItem()->getTypeItem(), 
			this->slotPiece->getItem()->getIdItem()));
		this->slotPiece->setEmpty();
		this->slotPiece->setSelected(false);
		this->gridViewPiece->deleteItemByIndex(this->indexRock);
		//
	}
	else
	{
		this->alert->setVisible(true);
		this->alert->setTitleAlert("SORRY, YOU NOT ENOUGH COIN TO \nUPGRADE THIS ITEM");
		this->intentId = 1;
	}
	
	this->firstActionButton->setVisible(false);

}

void SmithyScene::receiver(bool value)
{
	if (value)
	{
		switch (this->intentId)
		{
			case 1:
			{
					  this->intentId = 0;
					  break;
			}

			default:
				break;
		}
	}
	else
	{
		this->intentId = 0;
	}
	
	if (this->intentId == 0)
	{
		this->alert->setVisible(false);
	}
}