#include "Scenes\LoadingMapScene.h"
#include "GameManager\GameObjectManager.h"
#include <thread>
#include <mutex>

ui::LoadingBar* LoadingMapScene::loadingBar = NULL;
Label* LoadingMapScene::labelLoading = NULL;
bool LoadingMapScene::m_isChange = false;
std::string LoadingMapScene::m_fileDirect = "";

static bool isCreate = false;
Scene* LoadingMapScene::createScene(std::string file)
{
	m_fileDirect = file;

	auto scene = Scene::create();
	auto layer = LoadingMapScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingMapScene::init()
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

	isCreate = false;

	scheduleUpdate();
	return true;
}

void LoadingMapScene::update(float deltatime)
{
	if (!isCreate)
		thread([]() {
		GameObjectManager::getInstance()->createNewGame(m_fileDirect);
	}).detach(),
		isCreate = true;

	//CATCH IF LOADING PROGRESS IS DONE
	//Show message and hide loading bar to user
	if (loadingBar->getPercent() == 100.0 && loadingBar->isVisible())
	{
		//show message
		CCBlink* blink = CCBlink::create(1, 1);
		labelLoading->runAction(RepeatForever::create(blink));
		labelLoading->setString("- Tap to play -");
		labelLoading->enableOutline(Color4B::ORANGE, 2);
		//hide loading bar

		auto hide = CallFunc::create([&](){loadingBar->setVisible(false); loadingBarBg->setVisible(false); });
		auto sequence = Sequence::createWithTwoActions(FadeOut::create(0.25), hide);
		loadingBar->runAction(sequence);
		loadingBarBg->runAction(FadeOut::create(0.25));
	}
}

void LoadingMapScene::createLayout()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//ADD BACKGROUND FOR SCENE
	auto background = Sprite::create("UI/loading/background-loading-nontext.png");
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

void LoadingMapScene::setPercent(int percent)
{
	loadingBar->setPercent(percent);
	CCString* ccpercent = CCString::createWithFormat("%1.0f%%", loadingBar->getPercent());
	labelLoading->setString(ccpercent->getCString());
}

void LoadingMapScene::initListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(LoadingMapScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LoadingMapScene::onTouchBegan(Touch* touch, Event* event)
{
	if (loadingBar->getPercent() == 100.0) {
		//DISPLAY NEXT SCENE
		Director::getInstance()->resume();
		GameObjectManager::getInstance()->startGame();
	}

	return true;
}