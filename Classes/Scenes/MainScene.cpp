#include "MainScene.h"
#include "SettingOverlay.h"
#include "GUI\Alert.h"
#include "Import\AudioConfig.h"
#include "SimpleAudioEngine.h"
#include "CollectionScene.h"
#include "SmithyScene.h"
#include "ShopScene.h"
#include "TowerScene.h"
#include "MapScene.h"
Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}
int MainScene::state = 1;
bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(AudioConfig::getInstance()->getBackgroundVolume() / 100.0f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(AudioConfig::getInstance()->getEffectVolume() / 100.0f);
	AudioConfig::getInstance()->playTheme(0);

	createLayout();
	addListener();

	return true;
}

void MainScene::createLayout()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//ADD BACKGROUND
	auto background = Sprite::create("UI/main screen/background-main-menu.png");
	auto bgSize = background->getContentSize();
	auto raito = Vec2(visibleSize.width / bgSize.width, visibleSize.height / bgSize.height);
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setPosition(visibleSize / 2);
	background->setScale(visibleSize.width / bgSize.width, visibleSize.height / bgSize.height);
	addChild(background, -1);

	//*****************************************
	//		ADD BUILDING BUTTON
	//*****************************************
	//BUTTON SHOP
	btnShop = ui::Button::create("UI/main screen/icon-shop.png", "UI/main screen/icon-shop-pressed.png", "UI/main screen/icon-shop.png");
	btnShop->setScale(raito.x, raito.y);
	btnShop->setPosition(Vec2(visibleSize.width / 10, visibleSize.height * 9 / 20));
	addChild(btnShop, 0);
	//BUTTON TOWER
	btnTower = ui::Button::create("UI/main screen/icon-tower.png", "UI/main screen/icon-tower-pressed.png", "UI/main screen/icon-tower.png");
	btnTower->setScale(raito.x, raito.y);
	btnTower->setPosition(Vec2(visibleSize.width * 2 / 7, visibleSize.height * 7 / 25));
	addChild(btnTower, 0);
	//BUTTON ARENE
	btnArena = ui::Button::create("UI/main screen/icon-arena.png", "UI/main screen/icon-arena-pressed.png", "UI/main screen/icon-arena.png");
	btnArena->setScale(raito.x, raito.y);
	btnArena->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 15));
	addChild(btnArena, 0);
	//BUTTON SMITHY
	btnSmithy = ui::Button::create("UI/main screen/icon-smithy.png", "UI/main screen/icon-smithy-pressed.png", "UI/main screen/icon-smithy.png");
	btnSmithy->setScale(raito.x, raito.y);
	btnSmithy->setPosition(Vec2(visibleSize.width * 4 / 5, visibleSize.height / 4));
	addChild(btnSmithy, 0);
	//BUTTON PACK
	btnPack = ui::Button::create("UI/main screen/icon-pack.png", "UI/main screen/icon-pack-pressed.png", "UI/main screen/icon-pack.png");
	btnPack->setScale(raito.x, raito.y);
	btnPack->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height / 2));
	addChild(btnPack, 0);

	SettingOverlay* settingOverlay = new SettingOverlay();
	settingOverlay->setCallBack([&](bool visible){setBuidlingButtonDisplay(visible); });
	this->addChild(settingOverlay->getOverlay());
}

void MainScene::addListener()
{
	isMapRunning = false;
	btnArena->addClickEventListener([&](Ref* event){
		
		if (this->isMapRunning == false)
		{
			this->isMapRunning = true;
			openMap();
		}
			
	});

	//LISTENER FOR COLLECTION
	btnPack->addClickEventListener([&](Ref* event){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->pushScene(CollectionScene::createScene());
	});

	btnSmithy->addClickEventListener([](Ref* event){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->pushScene(SmithyScene::createScene());
	});

	btnShop->addClickEventListener([](Ref* event){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->pushScene(ShopScene::createScene());
	});
	btnTower->addClickEventListener([](Ref* event){
		AudioConfig::getInstance()->playEffect("OPENBUILDING");
		Director::getInstance()->pushScene(TowerScene::createScene());
	});
}


void MainScene::setBuidlingButtonDisplay(bool visible)
{
	btnArena->setEnabled(visible);
	btnShop->setEnabled(visible);
	btnPack->setEnabled(visible);
	btnSmithy->setEnabled(visible);
	btnTower->setEnabled(visible);
}

void MainScene::openMap()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//CREATE OPENING LAYER
	Layer* openingLayer = Layer::create();
	openingLayer->setContentSize(visibleSize);
	//add background color
	auto gray = Sprite::create("UI/gray.png");
	gray->setScale(visibleSize.width / gray->getContentSize().width, visibleSize.height / gray->getContentSize().height);
	gray->setPosition(visibleSize / 2);
	gray->setOpacity(60);
	openingLayer->addChild(gray, -1);
	//create scroll sprite
	
	//scroll right
	Sprite* scrollRight = Sprite::create("UI/button/scroll.png");
	scrollRight->setScale(visibleSize.height / scrollRight->getContentSize().height);
	scrollRight->setPosition(Vec2(visibleSize.width / 2 + scrollRight->getBoundingBox().size.width / 2, visibleSize.height / 2));
	openingLayer->addChild(scrollRight, 0);
	//scroll left
	Sprite* scrollLeft = Sprite::createWithTexture(scrollRight->getTexture());
	scrollLeft->setScale(visibleSize.height / scrollRight->getContentSize().height);
	scrollLeft->setPosition(Vec2(visibleSize.width / 2 - scrollLeft->getBoundingBox().size.width / 2, visibleSize.height / 2));
	openingLayer->addChild(scrollLeft, 0);
	//paper
	Sprite* scrollPaper = Sprite::create("UI/button/scroll-paper.png");
	scrollPaper->setScale(visibleSize.width / scrollPaper->getContentSize().width, visibleSize.height / scrollPaper->getContentSize().height);
	
	//action split from center to side
	auto splitEff = Sequence::createWithTwoActions(ProgressTo::create(0.5, 100), ProgressTo::create(0, 0));
	//logo
	Sprite* logoSprite = Sprite::create("UI/logo.png");
	logoSprite->setPosition(visibleSize / 2);
	logoSprite->runAction(Sequence::createWithTwoActions(ProgressTo::create(0.5, 100), ProgressTo::create(0, 0)));
	openingLayer->addChild(logoSprite, 2);
	ProgressTimer* Paper = ProgressTimer::create(scrollPaper);
	Paper->setType(ProgressTimer::Type::BAR);
	Paper->setScale(visibleSize.width / scrollPaper->getContentSize().width, visibleSize.height / scrollPaper->getContentSize().height);
	Paper->setMidpoint(Vec2(0.5f, 0.5f));
	Paper->setBarChangeRate(Vec2(1, 0));
	Paper->setPosition(visibleSize / 2);
	openingLayer->addChild(Paper, -1);
	//create action for paper effect
	auto showMap = CallFunc::create([&](){
		this->removeChildByName("openMap");
		this->removeChild(settingOverlay);
		this->isMapRunning = false;
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, MapScene::createScene()));

	});
	auto sequence = Sequence::createWithTwoActions(splitEff->clone(), showMap);
	Paper->runAction(sequence);
	//create action for scroll
	auto moveBy = MoveBy::create(0.5, Vec2(visibleSize.width / 2, 0));
	scrollRight->runAction(moveBy);
	scrollLeft->runAction(moveBy->clone()->reverse());
	openingLayer->setName("openMap");
	this->addChild(openingLayer, 6);
}