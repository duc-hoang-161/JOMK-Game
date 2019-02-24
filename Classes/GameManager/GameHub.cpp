#include "GameHub.h"

#include "Scenes\MapScene.h"
#include "Character\MainCharacter\MainCharacter.h"
#include "GameManager\GameObjectManager.h"
#include "Scenes\Demo2Scene.h"
#include "Import\PackConfig.h"

const int GameHub::BUTTON_LEFT = 1;
const int GameHub::BUTTON_RIGHT = 2;
const int GameHub::BUTTON_FIGHT = 3;
const int GameHub::BUTTON_JUMP = 4;
const int GameHub::BUTTON_FIRST_SKILL = 5;
const int GameHub::BUTTON_SECOND_SKILL = 6;

const int GameHub::WIN = 1;
const int GameHub::LOSE = 2;
const GLubyte DEF_OPACITY = 125;

static std::string _firstSkill = "";
static std::string _secondSkill = "";

GameHub::GameHub()
{
	m_hubLayer = Layer::create();
	m_hubLayer->retain();
	btnFirstSkill = NULL;
	btnSecSkill = NULL;
	m_mapLayer = NULL;
	createControlButton();
	createHealthAndPowerBar();
	createCoins();
}

void GameHub::createControlButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_settingOverlay = new SettingOverlay(SettingOverlay::FULL_OPERATION);
	m_settingOverlay->setCallBack([&](bool isEnable){
		setEnable(isEnable);
	});
	m_hubLayer->addChild(m_settingOverlay->getOverlay(), 2);

	btnFight = ui::Button::create("UI/button/fightbutton.png");
	btnFight->setOpacity(DEF_OPACITY);
	btnFight->setScale(2.5);
	Size fightBtnSize = btnFight->getBoundingBox().size;
	btnFight->setPosition(Vec2(visibleSize.width - fightBtnSize.width / 2, fightBtnSize.height / 2));
	btnFight->setTag(GameHub::BUTTON_FIGHT);
	m_hubLayer->addChild(btnFight, 1);

	btnJump = ui::Button::create("UI/button/jumpbutton.png");
	btnJump->setOpacity(DEF_OPACITY);
	btnJump->setScale(1.5);
	//Size fightBtnSize = btnJump->getBoundingBox().size;
	Vec2 fightPosition = btnFight->getPosition();
	btnJump->setPosition(Vec2(fightPosition.x - fightBtnSize.width, fightBtnSize.height / 2));
	btnJump->setTag(GameHub::BUTTON_JUMP);
	m_hubLayer->addChild(btnJump, 1);

	auto touchButton = [&](Ref *sender, ui::Widget::TouchEventType type) {
		auto node = dynamic_cast<ui::Button*>(sender);
		if (type == ui::Widget::TouchEventType::BEGAN)
			m_touchID[node->getTag()] = true;
		else if (type == ui::Widget::TouchEventType::MOVED) {
			if (!node->getBoundingBox().containsPoint(node->getTouchMovePosition()))
				m_touchID[node->getTag()] = false;
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
			m_touchID[node->getTag()] = false;
	};

	int joystickOffset = 10;
	Point origin = Point(100, 100);
	Rect joystickBaseDimensions = Rect(0, 0, 200.0f, 200.0f);
	Point joystickBasePosition = Point(origin.x + (joystickBaseDimensions.size.width / 2) + joystickOffset, origin.y + (joystickBaseDimensions.size.height / 2) + joystickOffset);
	Rect accelButtonDimensions = Rect(0, 0, 200.0f, 200.0f);
	Point accelButtonPosition = Point(origin.x + visibleSize.width - (joystickBaseDimensions.size.width / 2) - joystickOffset, origin.y + (joystickBaseDimensions.size.height / 2) + joystickOffset);
	// add joystick base
	SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::create();
	SneakyJoystick *joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystickBase->setBackgroundSprite(cocos2d::Sprite::create("UI/joy.png"));
	joystickBase->setThumbSprite(cocos2d::Sprite::create("UI/gray.png"));
	joystickBase->setScale(1.5);
	joystickBase->setJoystick(joystick);
	joystickBase->setPosition(joystickBasePosition);
	leftJoystick = joystickBase->getJoystick();
	m_hubLayer->addChild(joystickBase);
	leftJoystick->onVelocityChanged = [&, this](SneakyJoystick* eventJoystick, Point oldValue, Point newValue){
		if (newValue.x > 0.2 && newValue.y < 0.5 &&  newValue.y > -0.5)
		{
			this->m_touchID[GameHub::BUTTON_LEFT] = false;
			this->m_touchID[GameHub::BUTTON_RIGHT] = true;
		}
		else
		{
			if (newValue.x < -0.2 && newValue.y < 0.5 &&  newValue.y > -0.5)
			{
				this->m_touchID[GameHub::BUTTON_RIGHT] = false;
				this->m_touchID[GameHub::BUTTON_LEFT] = true;
			}
			else
			{
				this->m_touchID[GameHub::BUTTON_LEFT] = false;
				this->m_touchID[GameHub::BUTTON_RIGHT] = false;
			}
		}
	};

	GameItem* bluePotionItem = ItemsConfig::getInstance()->getItem(111, 1);
	GameItem* redPotionItem = ItemsConfig::getInstance()->getItem(111, 2);
	bluePotion = ui::Button::create(bluePotionItem->getSrcInGame());
	bluePotion->setPosition(Vec2(visibleSize.width - bluePotion->getContentSize().width, visibleSize.height * 3 / 4));
	bluePotion->setOpacity(DEF_OPACITY);
	bluePotion->setTitleText(CCString::createWithFormat("%d", PackConfig::getInstance()->getNumberPotionBlue())->getCString());
	bluePotion->setTitleFontSize(30);
	m_hubLayer->addChild(bluePotion);
	redPotion = ui::Button::create(redPotionItem->getSrcInGame());
	redPotion->setPosition(Vec2(bluePotion->getPositionX(), bluePotion->getPositionY() - redPotion->getContentSize().height*1.5));
	redPotion->setOpacity(DEF_OPACITY);
	redPotion->setTitleText(CCString::createWithFormat("%d", PackConfig::getInstance()->getNumberPotionRed())->getCString());
	redPotion->setTitleFontSize(30);
	m_hubLayer->addChild(redPotion);


	btnFight->addTouchEventListener(touchButton);
	btnJump->addTouchEventListener(touchButton);
	// btnFirstSkill->addTouchEventListener(touchButton);

	bluePotion->addClickEventListener([&](Ref* event){
		if (GameObjectManager::getInstance()->getMainCharacter()->getEnergy() < GameObjectManager::getInstance()->getMainCharacter()->getMaxEnergy())
		{
			if (PackConfig::getInstance()->usePotionBlue())
			{
				log("used a blue potion");
				GameObjectManager::getInstance()->getMainCharacter()->addEnergy(100);
			}
		}
	});

	redPotion->addClickEventListener([&](Ref* event){
		if (GameObjectManager::getInstance()->getMainCharacter()->getHP() < GameObjectManager::getInstance()->getMainCharacter()->getMaxHP())
		{
			if (PackConfig::getInstance()->usePotionRed())
			{
				log("used a red potion");
				GameObjectManager::getInstance()->getMainCharacter()->addHP(100);
			}
		}
	});

	m_energyBarBackground = Sprite::create("UI/healthBar.png");
	m_energyBar = ui::LoadingBar::create("UI/innerHealthBar-green.png");
	m_energyBarBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1.0 / 5));
	m_energyBar->setPosition(m_energyBarBackground->getPosition());
	m_energyBar->setPercent(0);
	m_energyBar->setDirection(ui::LoadingBar::Direction::LEFT);
	m_hubLayer->addChild(m_energyBarBackground);
	m_hubLayer->addChild(m_energyBar);
	m_energyBarBackground->setVisible(false);
	m_energyBar->setVisible(false);

	timeFirstSkill = Label::createWithTTF("", "UI/font/SVN-Prima.ttf", 25);
	timeFirstSkill->setVisible(false);
	timeSecondSkill = Label::createWithTTF("", "UI/font/SVN-Prima.ttf", 25);
	timeSecondSkill->setVisible(false);

	timeFirstSkill->setPosition(Vec2(fightPosition.x - fightBtnSize.width * 3 / 4, fightBtnSize.height * 4 / 3));
	timeSecondSkill->setPosition(Vec2(fightPosition.x, fightBtnSize.height * 5 / 3));

	m_hubLayer->addChild(timeFirstSkill);
	m_hubLayer->addChild(timeSecondSkill);
}

void GameHub::countDownLabel(Label *label, int time)
{
	if (label == timeFirstSkill) {
		if (btnFirstSkill->isEnabled())
			btnFirstSkill->setEnabled(false);
		else
			return;
	}
	else if (label == timeSecondSkill) {
		if (btnSecSkill->isEnabled())
			btnSecSkill->setEnabled(false);
		else
			return;
	}

	label->setVisible(true);
	label->setTag(time);

	Sequence *s = Sequence::createWithTwoActions(CallFunc::create([&, label]() {	
		CCString* ccpercent = CCString::createWithFormat("%d", label->getTag());
		label->setString(ccpercent->getCString());
		label->setTag(label->getTag() - 1);

		if (label->getTag() == -1) {
			if (label == timeFirstSkill)
				btnFirstSkill->setEnabled(true);
			else if (label == timeSecondSkill)
				btnSecSkill->setEnabled(true);

			label->setVisible(false);

			label->stopAllActions();
		}
	}), DelayTime::create(1));

	label->runAction(RepeatForever::create(s));
}

void GameHub::startEnergyBar()
{
	m_energyBarBackground->setVisible(true);
	m_energyBar->setVisible(true);

	m_energyBar->setPercent(25);
	auto updateLoadingBar = CallFunc::create([&]() {
		if (m_energyBar->getPercent() < 100)
		{
			m_energyBar->setPercent(m_energyBar->getPercent() + 1);
		}
	});

	auto sequenceRunUpdateLoadingBar = Sequence::createWithTwoActions(updateLoadingBar, DelayTime::create(0.008));
	auto repeat = Repeat::create(sequenceRunUpdateLoadingBar, 100);
	m_energyBar->runAction(repeat);
}

void GameHub::stopEnergyBar()
{
	m_energyBarBackground->setVisible(false);
	m_energyBar->setVisible(false);

	m_energyBar->stopAllActions();
}

void GameHub::createCoins()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* packImg = Sprite::create("items/pack.png");
	packImg->setPositionX(visibleSize.width * 3 / 4);
	packImg->setPositionY(visibleSize.height - packImg->getContentSize().height);
	//packImg->setOpacity(DEF_OPACITY);
	m_hubLayer->addChild(packImg);

	m_keyImage = Sprite::create("items/key/key.png");
	m_keyImage->setPositionX(packImg->getPosition().x + 100);
	m_keyImage->setPositionY(packImg->getPosition().y);
	m_hubLayer->addChild(m_keyImage);
	m_keyImage->setVisible(false);

	slotAvailable = Label::createWithTTF(
		CCString::createWithFormat("%d/%d", PackConfig::getInstance()->getItemsPack().size(), 
		PackConfig::getInstance()->getAvailableSlot())->getCString(), "UI/font/SVN-Prima.ttf", 30);
	slotAvailable->setAnchorPoint(Vec2(1, 0.5));
	slotAvailable->setPositionY(packImg->getPositionY());
	slotAvailable->setPositionX(packImg->getPositionX() - packImg->getContentSize().width / 2 - 10);
	m_hubLayer->addChild(slotAvailable);

	std::string value = StringUtils::format("%d", PackConfig::getInstance()->getCoins());
	m_coins = Label::createWithTTF(value, "UI/font/SVN-Prima.ttf", 30);
	m_coins->setPosition(Vec2(visibleSize.width / 2, packImg->getPositionY()));
	m_coins->setString(value);
	m_hubLayer->addChild(m_coins, 1);

	auto m_coinsImage = Sprite::create("opp-assets/UI/item/coin.png");
	//m_coinsImage->setOpacity(DEF_OPACITY);
	m_coinsImage->setAnchorPoint(Vec2(0,0.5));
	m_coinsImage->setPosition(Vec2(visibleSize.width / 2 + m_coins->getContentSize().width / 2 + 10, packImg->getPositionY()));
	m_hubLayer->addChild(m_coinsImage, 1);
}

void GameHub::removeMapLayer()
{
	if (m_mapLayer != NULL) {
		dynamic_cast<Demo2Scene*>(m_mapLayer)->stopUpdate();
		m_hubLayer->removeChild(m_mapLayer);
		m_mapLayer->release();
		m_settingOverlay->close();
		m_mapLayer = NULL;
	}
}

void GameHub::setVisibleKey(bool v)
{
	m_keyImage->setVisible(v);
}

bool GameHub::isHaveKey()
{
	return m_keyImage->isVisible();
}

void GameHub::setViewPointCenter(Point pos)
{
	m_pointCenter = pos;
}

void GameHub::setMapLayer(Layer *layer)
{
	GameObjectManager::getInstance()->getMainCharacter()->initialize();

	for (auto it = m_touchID.begin(); it != m_touchID.end(); ++it)
		it->second = false;

	timeFirstSkill->setVisible(false);
	timeSecondSkill->setVisible(false);

	// init
	m_keyImage->setVisible(false);

	setSkillButton();

	layer->retain();
	m_mapLayer = layer;
	m_hubLayer->addChild(layer, -1);
}

void GameHub::createHealthAndPowerBar()
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	Sprite* m_barBackground = Sprite::create("UI/bar-bg.png");
	m_barBackground->setPosition(Vec2(m_barBackground->getContentSize().width/2 + 10,
		screenSize.height - m_barBackground->getContentSize().height/2 - 10));
	//m_barBackground->setOpacity(DEF_OPACITY);
	m_hubLayer->addChild(m_barBackground);

	m_healthBar = ui::LoadingBar::create("UI/innerHealthBar-blue.png");
	m_powerBar = ui::LoadingBar::create("UI/innerHealthBar-green.png");

	m_healthBar->setPercent(100);
	//m_healthBar->setOpacity(DEF_OPACITY);
	m_healthBar->setDirection(ui::LoadingBar::Direction::LEFT);
	m_powerBar->setPercent(100);
	//m_powerBar->setOpacity(DEF_OPACITY);
	m_powerBar->setDirection(ui::LoadingBar::Direction::LEFT);

	m_healthBar->setPosition(Vec2(m_barBackground->getPositionX() + 42, m_barBackground->getPositionY() + 30));
	m_powerBar->setPosition(Vec2(m_barBackground->getPositionX() + 42, m_barBackground->getPositionY() - 2));

	m_hubLayer->addChild(m_healthBar);
	m_hubLayer->addChild(m_powerBar);
}

void GameHub::moveToCoins(Sprite *sprite, Vec2 positionInMap)
{
	auto parent = sprite->getParent();
	if (parent != NULL)
		parent->removeChild(sprite);
	bool visible = sprite->isVisible();

	sprite->setVisible(true);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	m_hubLayer->addChild(sprite);
	sprite->setPosition(ccpAdd(positionInMap, m_pointCenter));

	auto move = MoveTo::create(1, m_coins->getPosition());
	sprite->runAction(Sequence::create(move, CallFunc::create([&, sprite, parent, visible]() {
		m_hubLayer->removeChild(sprite);
		if (parent != NULL)
			parent->addChild(sprite);
		sprite->setVisible(visible);
	}), nullptr));
}

void GameHub::moveToBag(Sprite *sprite, Vec2 positionInMap)
{
	auto parent = sprite->getParent();
	if (parent != NULL)
		parent->removeChild(sprite);

	bool visible = sprite->isVisible();

	sprite->setVisible(true);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	m_hubLayer->addChild(sprite);
	Vec2 pim = positionInMap;
	Vec2 pc = m_pointCenter;
	sprite->setPosition(ccpAdd(positionInMap, m_pointCenter));
	
	auto move = MoveTo::create(1.5, this->slotAvailable->getPosition());
	sprite->runAction(Sequence::create(move, CallFunc::create([&, sprite, parent, visible]() {
		m_hubLayer->removeChild(sprite);
		if (parent != NULL)
			parent->addChild(sprite);
		sprite->setVisible(visible);
	}), nullptr));
}

void GameHub::update(float deltaTime)
{
	// energy bar
	if (m_startEnergyBar == true && (int)(clock() - t_startedEnergyBarTime) > 100)
		startEnergyBar(),
		m_startEnergyBar = false;

	MainCharacter *mc = GameObjectManager::getInstance()->getMainCharacter();
	if (!mc->isAlive())
	{
		this->m_settingOverlay->btnSetting->setEnabled(false);
	}
	m_healthBar->setPercent(mc->getHP() * 1.0 / mc->getMaxHP() * 100);
	m_powerBar->setPercent(mc->getEnergy() * 1.0 / mc->getMaxEnergy() * 100);

	std::string value = cocos2d::StringUtils::format("%d", PackConfig::getInstance()->getCoins());
	m_coins->setString(value);
	slotAvailable->setString(CCString::createWithFormat("%d/%d", PackConfig::getInstance()->getItemsPack().size(),
		PackConfig::getInstance()->getAvailableSlot())->getCString());
	bluePotion->setTitleText(CCString::createWithFormat("%d", PackConfig::getInstance()->getNumberPotionBlue())->getCString());
	redPotion->setTitleText(CCString::createWithFormat("%d", PackConfig::getInstance()->getNumberPotionRed())->getCString());
	if (mc->isAlive()) {
		bool isIDLE = true;
		if (isPress(GameHub::BUTTON_RIGHT))
			mc->moveRight(deltaTime), isIDLE = false;

		if (isPress(GameHub::BUTTON_LEFT))
			mc->moveLeft(deltaTime), isIDLE = false;

		if (isPress(GameHub::BUTTON_JUMP))
			mc->jumpUp(), isIDLE = false;

		if (isPress(GameHub::BUTTON_FIGHT))
			mc->hitWithStick(), isIDLE = false;

		if (isPress(GameHub::BUTTON_FIRST_SKILL))
			mc->runSkill(_firstSkill), countDownLabel(timeFirstSkill, mc->getTimeOfSkill(_firstSkill)), isIDLE = false;

		if (isPress(GameHub::BUTTON_SECOND_SKILL))
			mc->runSkill(_secondSkill), countDownLabel(timeSecondSkill, mc->getTimeOfSkill(_secondSkill)), isIDLE = false;

		if (isIDLE)
			mc->idle();
	}

	timeFirstSkill->update(deltaTime);
	timeSecondSkill->update(deltaTime);
}

cocos2d::Layer* GameHub::getLayer()
{
	return m_hubLayer;
}

void GameHub::showAlert(int MODE)
{
	if (!Director::getInstance()->isPaused())
		Director::getInstance()->pause();
	this->m_settingOverlay->setEnable(false);
	this->m_settingOverlay->getOverlay()->setVisible(false);
	this->setEnable(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//overlay gray
	LayerColor* overLay = LayerColor::create(Color4B(197, 197, 197, 107));
	overLay->setContentSize(visibleSize);
	m_hubLayer->addChild(overLay, 6);
	//alert window
	Sprite* alertWindow = Sprite::create("UI/button/alert-background.png");
	alertWindow->setContentSize(visibleSize / 2);
	alertWindow->setPosition(visibleSize / 2);
	overLay->addChild(alertWindow);
	Size windowSize = alertWindow->getContentSize();
	//title
	Label* titleAlert = Label::createWithTTF("ALMOST TO BE WINNER", "UI/font/UTM Karate.ttf", 40);
	titleAlert->setPosition(Vec2(alertWindow->getContentSize().width / 2, alertWindow->getContentSize().height * 3 / 4));
	alertWindow->addChild(titleAlert);
	//button left
	Button* button1 = new Button(Button::LIGHT_GRAY, "MAP", Color3B(142, 142, 0));
	button1->setPosition(Vec2(alertWindow->getContentSize().width / 4, alertWindow->getContentSize().height / 4));
	button1->getButton()->setScale(1.2);
	button1->getButton()->addClickEventListener([&, overLay](Ref* event) {
		setEnable(true);
		this->m_settingOverlay->setEnable(true);
		if (Director::getInstance()->isPaused())
			Director::getInstance()->resume();
		removeMapLayer();
		m_hubLayer->removeChild(overLay);
		Director::getInstance()->replaceScene(MapScene::createScene());
	});
	alertWindow->addChild(button1->getButton());
	//button right
	Button* button2 = new Button(Button::LIGHT_BROWN, "HOME", Color3B(215, 85, 0));
	button2->setPosition(Vec2(alertWindow->getContentSize().width * 3 / 4, alertWindow->getContentSize().height / 4));
	button2->getButton()->setScale(1.2);
	button2->getButton()->addClickEventListener([&, overLay](Ref* event){
		setEnable(true);
		this->m_settingOverlay->setEnable(true);
		if (Director::getInstance()->isPaused())
			Director::getInstance()->resume();
		removeMapLayer();
		m_hubLayer->removeChild(overLay);
		Director::getInstance()->replaceScene(MainScene::createScene());
	});
	alertWindow->addChild(button2->getButton());
	//set text
	if (MODE == WIN)
	{
		titleAlert->setString("YEAH, WINNER WINNER \nMONKEY WINNER");
		titleAlert->setTextColor(Color4B(160, 160, 0, 255));
		titleAlert->enableOutline(Color4B(158, 60, 0, 255), 2);
	}
	if (MODE == LOSE)
	{
		titleAlert->setTextColor(Color4B(175, 140, 0, 255));
		titleAlert->enableOutline(Color4B(28, 38, 48, 255), 2);
	}
}

void GameHub::setButtonTouch(int key, bool value)
{
	m_touchID[key] = value;
}

void GameHub::setEnable(bool isEnable)
{
	bluePotion->setEnabled(isEnable);
	redPotion->setEnabled(isEnable);
	if (btnFirstSkill != NULL)
		btnFirstSkill->setEnabled(isEnable);
	if (btnSecSkill != NULL)
		btnSecSkill->setEnabled(isEnable);
	btnJump->setEnabled(isEnable);
	btnFight->setEnabled(isEnable);
	if (!isEnable)
	{
		Director::getInstance()->pause();
	}
	else
	{
		if (Director::getInstance()->isPaused())
		{
			Director::getInstance()->resume();
		}
	}
}

bool GameHub::isPress(int buttonID)
{
	return m_touchID[buttonID];
}

GameHub::~GameHub()
{
	/*setEnable(true);
	m_hubLayer->release();
	delete m_settingOverlay;*/

	delete m_settingOverlay;
	m_hubLayer->release();
}

void GameHub::setSkillButton()
{
	//CREATE SKILL BUTTON
	log("create new skill button");
	Size fightBtnSize = btnFight->getBoundingBox().size;
	Vec2 fightPosition = btnFight->getPosition();

	std::string src = PackConfig::getInstance()->getFirstSkill() != NULL ? PackConfig::getInstance()->getFirstSkill()->getSrcInGame() : "";
	if (btnFirstSkill != NULL)
	{
		btnFirstSkill->removeFromParentAndCleanup(true);
	}
	if (src.length() < 5)
		src = "skill/ironFist/lv1-IG.png";
	btnFirstSkill = ui::Button::create(src);
	btnFirstSkill->setOpacity(DEF_OPACITY);
	btnFirstSkill->setScale(1.5);
	btnFirstSkill->setPosition(Vec2(fightPosition.x - fightBtnSize.width * 3 / 4, fightBtnSize.height * 4 / 3));
	btnFirstSkill->setTag(GameHub::BUTTON_FIRST_SKILL);
	setSkillButtonAction(btnFirstSkill, PackConfig::getInstance()->getFirstSkill()->getIdItem(), true);
	m_hubLayer->addChild(btnFirstSkill, 1);

	src = PackConfig::getInstance()->getSecSkill() != NULL ? PackConfig::getInstance()->getSecSkill()->getSrcInGame() : "";
	if (src.length() < 5)
		src = "skill/rageMode/lv1-IG.png";
	if (btnSecSkill != NULL)
	{
		btnSecSkill->removeFromParentAndCleanup(true);
	}
	btnSecSkill = ui::Button::create(src);
	btnSecSkill->setOpacity(DEF_OPACITY);
	btnSecSkill->setScale(1.5);
	btnSecSkill->setPosition(Vec2(fightPosition.x, fightBtnSize.height * 5 / 3));
	btnSecSkill->setTag(GameHub::BUTTON_SECOND_SKILL);
	setSkillButtonAction(btnSecSkill, PackConfig::getInstance()->getSecSkill()->getIdItem(), false);
	m_hubLayer->addChild(btnSecSkill, 1);	
}

void GameHub::setSkillButtonAction(ui::Button* btn, int idSkill, bool isFirst)
{
	std::string *pSkill = isFirst ? &_firstSkill : &_secondSkill;

	auto customTouchButton = [&](Ref *sender, ui::Widget::TouchEventType type) {
		auto node = dynamic_cast<ui::Button*>(sender);
		Vec2 point = node->getTouchMovePosition();
		static bool isExecute;
		if (type == ui::Widget::TouchEventType::BEGAN)
			m_startEnergyBar = true,
			t_startedEnergyBarTime = clock();
		else if (type == ui::Widget::TouchEventType::MOVED) {
			m_touchID[GameHub::BUTTON_JUMP] = false;
			if (btnJump->getBoundingBox().containsPoint(point))
				m_touchID[GameHub::BUTTON_JUMP] = true;
		}
		else if (type == ui::Widget::TouchEventType::CANCELED || type == ui::Widget::TouchEventType::ENDED) {
			int percent = m_energyBar->getPercent();
			int type = MainCharacter::BULLET_SMALL;
			if (percent > 50)
				type = MainCharacter::BULLET_MEDIUM;
			if (percent > 80)
				type = MainCharacter::BULLET_BIG;

			GameObjectManager::getInstance()->getMainCharacter()->setCurrentBulletType(type);
			GameObjectManager::getInstance()->getMainCharacter()->runSkill("IRON_FIST"),
				stopEnergyBar(),
				m_startEnergyBar = false;
			m_touchID[GameHub::BUTTON_JUMP] = false;
			m_energyBar->setPercent(0);
		}
	};

	auto touchButton = [&](Ref *sender, ui::Widget::TouchEventType type) {
		auto node = dynamic_cast<ui::Button*>(sender);
		if (type == ui::Widget::TouchEventType::BEGAN)
			m_touchID[node->getTag()] = true;
		else if (type == ui::Widget::TouchEventType::MOVED) {
			if (!node->getBoundingBox().containsPoint(node->getTouchMovePosition()))
				m_touchID[node->getTag()] = false;
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
			m_touchID[node->getTag()] = false;
	};

	//if flash skill
	if (idSkill >= 5 && idSkill <= 9)
	{
		*pSkill = "FLASH";
		GameObjectManager::getInstance()->getMainCharacter()->setLevelSkill("FLASH", idSkill - 5);
		btn->addTouchEventListener(touchButton);
	}

	//if heal skill
	if (idSkill >= 10 && idSkill <= 14)
	{
		*pSkill = "HEAL";
		GameObjectManager::getInstance()->getMainCharacter()->setLevelSkill("HEAL", idSkill - 10);
		btn->addTouchEventListener(touchButton);
	}

	//if iron fist skill
	if (idSkill >= 15 && idSkill <= 19)
	{
		*pSkill = "IRON_FIST";
		GameObjectManager::getInstance()->getMainCharacter()->setLevelSkill("IRON_FIST", idSkill - 15);
		btn->addTouchEventListener(customTouchButton);
	}

	//if rage mode skill
	if (idSkill >= 20 && idSkill <= 24)
	{
		*pSkill = "RAGE_MODE";
		GameObjectManager::getInstance()->getMainCharacter()->setLevelSkill("RAGE_MODE", idSkill - 20);
		btn->addTouchEventListener(touchButton);
	}

}