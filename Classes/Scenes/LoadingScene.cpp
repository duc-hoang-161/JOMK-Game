#include "Scenes\LoadingScene.h"
#include "GameManager\GameObjectManager.h"
#include <thread>
#include <mutex>

ui::LoadingBar* LoadingScene::loadingBar = NULL;
Label* LoadingScene::labelLoading = NULL;

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	AudioConfig::getInstance();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//KEEP SCREEN ON
	cocos2d::Device::setKeepScreenOn(true);
	//CREATE LAYOUT SCENE
	createLayout();
	//INIT TOUCH LISTENER
	initListener();

	//TEST LOADING BAR
	loadingBar->setPercent(0);
	auto updateLoadingBar = CallFunc::create([&](){
		int maxx = loadingBar->getTag();
		if (loadingBar->getPercent() < maxx)
		{
			loadingBar->setPercent(loadingBar->getPercent() + 5);
			CCString* percent = CCString::createWithFormat("%1.0f%%", loadingBar->getPercent());
			labelLoading->setString(percent->getCString());
		}
	});	

	auto testLoading = Sequence::createWithTwoActions(updateLoadingBar, DelayTime::create(0.1f));
	auto testLoadingTime = RepeatForever::create(testLoading);
	// runAction(testLoadingTime);

	thread([]() {
		GameObjectManager::getInstance();
	}).detach();

	scheduleUpdate();
	return true;
}

void LoadingScene::update(float deltatime)
{
	//CATCH IF LOADING PROGRESS IS DONE
	//Show message and hide loading bar to user
	if (loadingBar->getPercent() == 100.0 && loadingBar->isVisible())
	{
		//show message
		CCBlink* blink = CCBlink::create(1, 1);
		labelLoading->runAction(RepeatForever::create(blink));
		labelLoading->setString("- Tap on screen to continue -");
		labelLoading->enableOutline(Color4B::ORANGE, 2);
		//hide loading bar
		
		auto hide = CallFunc::create([&](){loadingBar->setVisible(false); loadingBarBg->setVisible(false); });
		auto sequence = Sequence::createWithTwoActions(FadeOut::create(0.25), hide);
		loadingBar->runAction(sequence);
		loadingBarBg->runAction(FadeOut::create(0.25));
		
	}
	
}

void LoadingScene::createLayout()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//ADD BACKGROUND FOR SCENE
	auto background = Sprite::create("UI/loading/background-loading.png");
	auto bgSize = background->getContentSize();
	auto raito = Vec2(visibleSize.width / bgSize.width, visibleSize.height / bgSize.height);
	
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setPosition(visibleSize / 2);
	background->setScale(visibleSize.width / bgSize.width, visibleSize.height / bgSize.height);
	addChild(background, -1);

	//ADD BACKGROUND FOR LOADING BAR
	loadingBarBg = Sprite::create("UI/loading/loading-unload.png");
	loadingBarBg->setPosition(visibleSize.width / 2, visibleSize.height / 8);
	loadingBarBg->setScale(raito.x, raito.y);
	addChild(loadingBarBg, 0);
	//ADD LOADING BAR
	loadingBar = ui::LoadingBar::create("UI/loading/loading-loaded.png");
	loadingBar->setPercent(0);
	loadingBar->setScale(raito.x, raito.y);
	loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingBar->setPosition(loadingBarBg->getPosition());
	addChild(loadingBar, 1);
	//ADD LABEL PROGRESS PERCENT
	labelLoading = Label::createWithTTF("0", "UI/font/SVN-Prima.ttf", 25);
	labelLoading->setPosition(Vec2(loadingBar->getPositionX(), loadingBar->getPositionY() + loadingBar->getBoundingBox().size.height));
	addChild(labelLoading, 1);
}

void LoadingScene::setPercent(int percent)
{
	loadingBar->setPercent(percent);
	CCString* ccpercent = CCString::createWithFormat("%1.0f%%", loadingBar->getPercent());
	labelLoading->setString(ccpercent->getCString());
}

void LoadingScene::initListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LoadingScene::onTouchBegan(Touch* touch, Event* event)
{
	if (loadingBar->getPercent() == 100.0) {
		//DISPLAY NEXT SCENE
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(TransitionFade::create(1, MainScene::createScene()));		
	}

	return true;
}