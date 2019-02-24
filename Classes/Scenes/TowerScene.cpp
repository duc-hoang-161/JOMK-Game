#include "Scenes\TowerScene.h"

Scene* TowerScene::createScene()
{
	return TowerScene::create();
}

bool TowerScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	this->selectedCol = this->selectedRow = -1;
	this->addChild(this->mainLayer, 0);
	setBackground("UI/button/background-2.png");
	LayerColor* splitLayer = LayerColor::create(Color4B(4, 7, 16, 255));
	splitLayer->setContentSize(Size(10, this->contentBackground->getContentSize().height * 113 / 125));
	splitLayer->setPosition(Vec2(this->contentBackground->getContentSize().width * 2 / 3, this->contentBackground->getContentSize().height * 6 / 125));
	this->contentBackground->addChild(splitLayer);
	this->setTitleHeader("SKILL TOWER");
	this->setIconUnit("UI/button/magic-point.png");
	this->setNumberValue(PackConfig::getInstance()->getMagicPoint());
	this->btnBack->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		//GameObjectManager::getInstance()->getGameHub()->setSkillButton();
		Director::getInstance()->popScene();
	});
	createLayout();
	setActionButton();
	createPersonalLayout();
	//create skill tree
	std::vector<std::vector<MagicItem*>> listSkill = ItemsConfig::getInstance()->getSkillList();
	for (int i = 0; i < listSkill.size(); i++)
	{
		Layer* skillLayer = this->createSkillTree(listSkill[i], i);
		skillLayer->setPosition(Vec2(32, 64 + i*96));
		this->scrollSkill->addChild(skillLayer);
	}
		

	return true;
}

void TowerScene::createLayout()
{
	//create left side layer for skill tree
	this->skillTreeLayer = Layer::create();
	this->skillTreeLayer->setContentSize(Size(this->contentLayer->getContentSize().width * 2 / 3, this->contentLayer->getContentSize().height));
	this->contentLayer->addChild(this->skillTreeLayer);
	//create scroll view for left side
	this->scrollSkill = ui::ScrollView::create();
	this->scrollSkill->setContentSize(this->skillTreeLayer->getContentSize());
	this->scrollSkill->setInnerContainerSize(this->skillTreeLayer->getContentSize());
	this->scrollSkill->setScrollBarAutoHideEnabled(false);
	//this->scrollSkill->setScrollBarAutoHideTime(0);
	this->scrollSkill->setBackGroundColor(Color3B::BLUE);
	this->scrollSkill->setAnchorPoint(Vec2(0, 0));
	this->scrollSkill->setLayoutType(ui::Layout::Type::VERTICAL);
	this->scrollSkill->setDirection(ui::ScrollView::Direction::VERTICAL);
	this->scrollSkill->setClippingEnabled(true);
	this->contentLayer->addChild(this->scrollSkill);

	this->detailBackground->setVisible(false);

	//create toast text
	this->toastText = Label::createWithTTF("+ NEW ITEM", "UI/font/000 Meanwhile.ttf", 40);
	this->toastText->setTextColor(Color4B::YELLOW);
	this->toastText->setVisible(false);
	this->toastText->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(this->toastText, 5);
	//create alert
	alert = new Alert("THIS IS ALERT", "", Alert::CONFIRM_SIZE);
	this->alert->setVisible(false);
	this->alert->getRightActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(false);
	});
	this->alert->getLeftActionButton()->getButton()->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		receiver(true);
	});
	this->addChild(alert->getAlertLayer(), 5);
}

Layer* TowerScene::createSkillTree(std::vector<MagicItem*> skills, int row)
{
	//create a skill line
	LayerColor* skillTree = LayerColor::create(Color4B(18, 39, 34, 255));
	Sprite* selected = Sprite::create("UI/button/slot-selected2.png");
	this->scrollSkill->addChild(selected);
	selected->setVisible(false);
	selected->setName("selected");
	//vector save skill line slots
	std::vector<Slot*> skillLine;
	//get maximum level this skill
	Vec2 levelSkill = PackConfig::getInstance()->getLevel(row);
	//create skill
	for (int i = 0; i < skills.size(); i++)
	{
		Slot* skill = new Slot(Slot::SLOT_SKILL);
		skill->setItem(skills[i]);
		if (i == 0)
		{
			skill->addToParent(skillTree, 1, Vec2(32 + 8 + 96 * i, 32 + 8), selected, row * 10 + i, [&, skill](int id, bool is){onItemClicked(id, is); });
		}
		else
		{
			skill->addToParent(skillTree, 1, Vec2(skillLine[i-1]->getPosition().x + 8 + 96 * i, 32 + 8), selected, row * 10 + i, [&, skill](int id, bool is){onItemClicked(id, is); });
		}
		if (skills[i]->getIdItem() <= levelSkill.y)
			skill->setChoose(true);
		else
		{
			skill->setChoose(false);
		}

		if (i != skills.size() - 1)
		{
			Sprite* upgradeArrow = Sprite::create("UI/button/arrow.png");
			upgradeArrow->setPosition(Vec2(skill->getPosition().x + 96 * i +56, 32 + 8));
			skillTree->addChild(upgradeArrow);
		}
		skillLine.push_back(skill);
	}
	skillTree->setContentSize(Size(464, 80));
	slotList.push_back(skillLine);
	this->lineSkillLayer.push_back(skillTree);
	return skillTree;
}

void TowerScene::setActionButton()
{
	//FIRST BUTTON
	this->firstActionButton->setTitleText("USE");
	this->firstActionButton->getButton()->addClickEventListener([&, this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		// if have any skill selected
		
		if (this->selectedCol != -1 && this->selectedRow != -1)
		{
			/*if (this->selectedRow == 0 || this->selectedRow == 1)
			{
				this->toastAText("Sorry this skill still in development, not use able");
				return;
			}*/
			//if slot 5 and 6 are empty then push item to 5
			if (this->slotBody[5].getItem() == NULL)
			{
				this->slotBody[5].setItem(this->slotList[this->selectedRow][this->selectedCol]->getItem());
				PackConfig::getInstance()->setEquipItemAt(5, 0, Vec2(this->slotList[this->selectedRow][this->selectedCol]->getItem()->getTypeItem(), this->slotList[this->selectedRow][this->selectedCol]->getItem()->getIdItem()));
			}
			else
			{
				//if slot 6 is empty
				if (this->slotBody[6].getItem() == NULL)
				{
					//if push same item
					if (this->slotBody[5].getItem() == this->slotList[this->selectedRow][this->selectedCol]->getItem())
					{
						this->alert->setVisible(true);
						this->alert->setTitleAlert("YOU CAN NOT USE SAME SKILL \nWITH SAME LEVEL");
						this->intentId = 2;
					}
					else
					{
						this->slotBody[6].setItem(this->slotList[this->selectedRow][this->selectedCol]->getItem());
						PackConfig::getInstance()->setEquipItemAt(6, 0, Vec2(this->slotList[this->selectedRow][this->selectedCol]->getItem()->getTypeItem(), this->slotList[this->selectedRow][this->selectedCol]->getItem()->getIdItem()));
					}
				}
				else
				{
					//if slot 5 and 6 is not available then push item from 6 to 5 and push new item to 6
					if (this->slotBody[5].getItem() != NULL && this->slotBody[6].getItem() != NULL)
					{
						//if push same item
						if (this->slotBody[6].getItem() == this->slotList[this->selectedRow][this->selectedCol]->getItem())
						{
							this->alert->setVisible(true);
							this->alert->setTitleAlert("YOU CAN NOT USE SAME SKILL \nWITH SAME LEVEL");
							this->intentId = 2;
						}
						else
						{
							this->slotBody[5].setItem(this->slotBody[6].getItem());
							PackConfig::getInstance()->setEquipItemAt(5, 0, Vec2(this->slotBody[6].getItem()->getTypeItem(), this->slotBody[6].getItem()->getIdItem()));
							this->slotBody[6].setItem(this->slotList[this->selectedRow][this->selectedCol]->getItem());
							PackConfig::getInstance()->setEquipItemAt(6, 0, Vec2(this->slotList[this->selectedRow][this->selectedCol]->getItem()->getTypeItem(), this->slotList[this->selectedRow][this->selectedCol]->getItem()->getIdItem()));
						}
					}
				}
			}
		}		
	});
	this->firstActionButton->setVisible(false);
	//SECOND BUTTON
	this->secActionButton->setVisible(false);
	//THIRD BUTTON
	this->thirdActionButton->setTitleText("UPGRADE");
	this->thirdActionButton->getButton()->addClickEventListener([&, this](Ref* event){
		//if have any item selected
		if (this->selectedCol != -1 && this->selectedRow != -1)
		{
			std::vector<Slot*> lineSkill = this->slotList[this->selectedRow];
			for (int i = 0; i < 5; i++)
			{
				//find first skill not upgrade yet
				if (!lineSkill[i]->isAvailable())
				{
					//if points are enough for upgrade
					if (PackConfig::getInstance()->getMagicPoint() >= i + 1)
					{
						AudioConfig::getInstance()->playEffect("SUCCESS");
						if (i == 4)
							this->thirdActionButton->setVisible(false);
						lineSkill[i]->setChoose(true);
						PackConfig::getInstance()->setLevel(this->selectedRow, lineSkill[i]->getItem()->getIdItem());
						PackConfig::getInstance()->subMagicPoint(i + 1);
						this->setNumberValue(PackConfig::getInstance()->getMagicPoint());
						toastAText("+ Upgrade successful");
					}
					else
					{
						this->alert->setTitleAlert("SORRY, YOU NOT ENOUGH MAGIC POINT \nTO UPGRADE THIS SKILL");
						this->alert->setVisible(true);
						this->intentId = 1;
						this->alert->getLeftActionButton()->setTitleText("BUY");
					}
					break;
				}					
			}			
		}
	});
	this->thirdActionButton->setVisible(false);
}

void TowerScene::onItemClicked(int id, bool value)
{
	int row = (int)id / 10;
	int col = (int)id % 10;
	int index = row * 5 + col;
	//if select skill
	if (value)
	{
		//set button use visible when choose skill had upgraded
		if (this->slotList[row][col]->isAvailable())
			this->firstActionButton->setVisible(true);
		else
		{
			this->firstActionButton->setVisible(false);
		}
		Slot* slot = slotList[row][col];
		this->detailBackground->setVisible(true);
		this->detailText->setString(slot->getItem()->showInfo());
		this->scrollDetail->setInnerContainerSize(Size(this->scrollDetail->getInnerContainerSize().width, this->detailText->getContentSize().height));
		//set button upgrade visible when the line skill not full upgrade yet
		if (!this->slotList[row][4]->isAvailable())
			this->thirdActionButton->setVisible(true);
		else
			this->thirdActionButton->setVisible(false);
		//set only one line can be selected
		this->lineSkillLayer[row]->runAction(cocos2d::TintTo::create(0.001, 42, 91, 80));
		for (int i = 0; i < lineSkillLayer.size(); i++)
		{
			if (i != row)
			{
				this->lineSkillLayer[i]->runAction(cocos2d::TintTo::create(0.001, 18, 39, 34));
			}
		}
		//set only one slot can be selected
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (!(i == row && j == col))
					slotList[i][j]->setSelected(false);
			}
		}
		this->selectedCol = col;
		this->selectedRow = row;
	}
	else
	{	//if un-select skill
		this->selectedCol = this->selectedRow = -1;
		this->firstActionButton->setVisible(false);
		this->detailBackground->setVisible(false);
		this->thirdActionButton->setVisible(false);
		this->lineSkillLayer[row]->runAction(cocos2d::TintTo::create(0.001, 18, 39, 34));
	}
}

void TowerScene::setUpgrade(Vec2 id, int row)
{

}

void TowerScene::toastAText(std::string mes)
{
	this->toastText->stopAllActions();
	this->toastText->setVisible(true);
	this->toastText->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->toastText->setOpacity(255);
	this->toastText->setString(mes);
	this->toastText->runAction(Spawn::createWithTwoActions(Repeat::create(MoveBy::create(0.5, Vec2(0, 150)), 1), Repeat::create(FadeOut::create(0.5), 1)));
}

void TowerScene::receiver(bool value)
{
	if (value)
	{
		switch (this->intentId)
		{
			//if not enough point to upgrade
			case 1:
			{
				this->intentId = 0;
				break;
			}
			//if push same skill same level
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
	}
	else
	{
		this->intentId = 0;
	}
	//reset alert
	if (this->intentId == 0)
	{
		this->alert->setVisible(false);
		this->alert->getLeftActionButton()->setTitleText("OK");
	}
}


void TowerScene::createPersonalLayout()
{
	//create left side layer
	Layer* leftSide = Layer::create();
	leftSide->setContentSize(Size(contentLayer->getContentSize().width / 3, contentLayer->getContentSize().height));
	leftSide->setPosition(Vec2(contentLayer->getContentSize().width * 2 / 3, 0));
	contentLayer->addChild(leftSide);
	//create person image
	Sprite* person = Sprite::create("UI/person.png");
	person->setScale(1.3);
	person->setPosition(Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height*3 / 5));
	leftSide->addChild(person);
	//create selected image
	Sprite* selected = Sprite::create("UI/button/slot-selected.png");
	selected->setVisible(false);
	leftSide->addChild(selected);
	//create slot for main character equip
	this->slotBody = new Slot[7];
	for (int i = 0; i < 5; i++)
	{
		slotBody[i].setOpacity(90);
		slotBody[i].setEnable(false);
	}
	//head
	slotBody[0].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 4.1 / 5), selected, 0, [&](int id, bool isSelected){});
	//hand left screen
	slotBody[1].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *0.4 / 2, leftSide->getContentSize().height * 2.8 / 5), selected, 0, [&](int id, bool isSelected){});
	//hand right screen
	slotBody[2].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *1.6 / 2, leftSide->getContentSize().height * 2.8 / 5), selected, 0, [&](int id, bool isSelected){});
	//body
	slotBody[3].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 3.2 / 5), selected, 0, [&](int id, bool isSelected){});
	//foot
	slotBody[4].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width / 2, leftSide->getContentSize().height * 1.7 / 5), selected, 0, [&](int id, bool isSelected){});
	//skill 1 left screen
	slotBody[5].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *0.4 / 2, leftSide->getContentSize().height * 0.8 / 5), selected, 0, [&](int id, bool isSelected){});
	//slotBody[5].setEnable(false);
	//skill 2 right screen
	slotBody[6].addToParent(leftSide, 1, Vec2(leftSide->getContentSize().width *1.6 / 2, leftSide->getContentSize().height * 0.8 / 5), selected, 0, [&](int id, bool isSelected){});
	//slotBody[6].setEnable(false);

	for (int i = 0; i < 7; i++)
	{
		if (PackConfig::getInstance()->getEquipItemAt(i) != NULL)
			slotBody[i].setItem(PackConfig::getInstance()->getEquipItemAt(i));
		else
			log("error equip");
	}
}
