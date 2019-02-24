#include "Scenes\MapScene.h"
#include "ui\UIScrollView.h"
#include "Import\MapPointConfig.h"
#include"Scenes\Demo2Scene.h"
#include "Scenes\MainScene.h"
bool isUnlockNewPoint = false;
Scene* MapScene::createScene()
{
	return MapScene::create();
}

Scene* MapScene::createScene(bool isUnlock)
{
	isUnlockNewPoint = isUnlock;
	return MapScene::create();
}

bool MapScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	createMapScene();
	addPointLevel();
	return true;
}

void MapScene::createMapScene()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//CREATE LAYER MAPP CHOOSE
	this->mapLayer = Layer::create();
	this->mapLayer->setContentSize(visibleSize);
	this->addChild(this->mapLayer, 0);
	//add map into layer map
	map = Sprite::create("UI/button/map.png");
	map->setAnchorPoint(Vec2(0, 0));
	map->setPosition(Vec2::ZERO);
	//add scroll
	scrollMap = ui::ScrollView::create();
	scrollMap->setContentSize(visibleSize);
	scrollMap->setPosition(visibleSize / 2);
	scrollMap->setDirection(ui::ScrollView::Direction::BOTH);
	scrollMap->setBackGroundColor(Color3B::BLUE);
	scrollMap->setScrollBarAutoHideEnabled(false);
	scrollMap->setScrollBarOpacity(100);
	scrollMap->setClippingEnabled(true);
	scrollMap->setAnchorPoint(Vec2(0.5, 0.5));
	scrollMap->setLayoutType(ui::Layout::Type::VERTICAL);
	scrollMap->setInnerContainerSize(map->getContentSize());
	scrollMap->addChild(map);
	this->addChild(scrollMap, 0);
	//add back button
	this->btnBack = new Button(Button::BACK_YELLOW, "     Back", Color3B(234, 107, 10));
	this->btnBack->setPosition(Vec2(visibleSize.width - btnBack->getButton()->getContentSize().width, visibleSize.height - btnBack->getButton()->getContentSize().height));
	this->btnBack->getButton()->addClickEventListener([](Ref* event){
		Director::getInstance()->replaceScene(TransitionFade::create(1.5, MainScene::createScene()));
	});
	this->addChild(btnBack->getButton(), 1);
	this->toastText = Label::createWithTTF("+ NEW ITEM", "UI/font/000 Meanwhile.ttf", 40);
	this->toastText->setTextColor(Color4B::YELLOW);
	this->toastText->setVisible(false);
	//this->toastText->setPosition();
	this->addChild(this->toastText, 55);
}

#include "Scenes\LoadingMapScene.h"
void MapScene::runMap(int mapLevel)
{
	log("RUN MAP %d", mapLevel);
	switch (mapLevel)
	{
		case 1:
		{
			GameObjectManager::getInstance()->setCurrentPlayLevel(1);
			// Director::getInstance()->replaceScene(Demo2Scene::createScene("opp-assets/Map01.tmx", "opp-assets/Map01Setup.json"));
			//Director::getInstance()->replaceScene(Demo2Scene::createScene("opp-assets/Map01.tmx", "opp-assets/Map01Setup.json"));
			Director::getInstance()->replaceScene(LoadingMapScene::createScene("opp-assets/Map01Setup.json"));
			break;
		}
		case 2:
		{
			GameObjectManager::getInstance()->setCurrentPlayLevel(2);
			//Director::getInstance()->replaceScene(Demo2Scene::createScene("opp-assets/Map02.tmx", "opp-assets/Map02Setup.json"));
			Director::getInstance()->replaceScene(LoadingMapScene::createScene("opp-assets/Map02Setup.json"));
			break;
		}
		case 3:
		{
			//use Cave Map 
			GameObjectManager::getInstance()->setCurrentPlayLevel(3);
			//Director::getInstance()->replaceScene(Demo2Scene::createScene("opp-assets/Map03.tmx", "opp-assets/Map03Setup.json"));
			Director::getInstance()->replaceScene(LoadingMapScene::createScene("opp-assets/Map03Setup.json"));
			break;
		}
		case 4:
		{
			GameObjectManager::getInstance()->setCurrentPlayLevel(4);
			//Director::getInstance()->replaceScene(Demo2Scene::createScene("opp-assets/Map04.tmx", "opp-assets/Map04Setup.json"));
			Director::getInstance()->replaceScene(LoadingMapScene::createScene("opp-assets/Map04Setup.json"));
			break;
		}
		default:
			GameObjectManager::getInstance()->setCurrentPlayLevel(0);
			break;
	}
}

void MapScene::addPointLevel()
{
	//GET MAP POINT
	//total point on map
	std::vector<Vec2> vectorPoint = MapPointConfig::getInstance()->getMapPointConfig();
	//add passed point
	int numPassedPoint = PackConfig::getInstance()->getPassedPoint();
	for (int i = 0; i < (int)vectorPoint.size(); i++)
	{
		if (i < numPassedPoint)
		{
			ui::Button* flagPassed;
			//create flag passed
			if (i < numPassedPoint - 1)
			{
				flagPassed = ui::Button::create("UI/button/flag.png", "UI/button/flag-pressed.png", "UI/button/flag.png");
				if (isUnlockNewPoint && i == numPassedPoint - 2)
				{
					//visible button
					flagPassed->setVisible(false);
					//add old icon flag
					Sprite* oldCurrent = Sprite::create("UI/button/flag-next.png");
					oldCurrent->setPosition(vectorPoint.at(i));
					map->addChild(oldCurrent);
					//add mc icon
					Sprite* monk = Sprite::create("UI/54.png");
					monk->setPosition(vectorPoint.at(i));
					monk->setVisible(false);
					map->addChild(monk, 4);
					//add effect win at point
					auto particle = ParticleSystemQuad::create("res/SmallSun.plist");
					particle->setTotalParticles(100);
					particle->setPosition(vectorPoint.at(i));
					particle->setStartColor(Color4F(1, 0, 0, 1));
					particle->setBlendAdditive(false);
					particle->setScale(0.1);
					map->addChild(particle, 10);

					
					auto s1 = CallFunc::create([&, monk, flagPassed, particle, oldCurrent, vectorPoint, i, this](){
						this->setEnabled(false);
						monk->setVisible(true);
						particle->setVisible(false);
						flagPassed->setVisible(true);
						flagPassed->runAction(FadeIn::create(0.5));
						oldCurrent->setVisible(false);
						monk->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0, vectorPoint.at(i + 1)), FadeOut::create(0.1)));
					});
					//run eff win point
					auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.7, 4), s1);
					particle->runAction(seq);
				}
			}
			//create current passed
			else
			{
				flagPassed = ui::Button::create("UI/button/flag-next.png", "UI/button/flag-next-pressed.png", "UI/button/flag-next.png");

				if (isUnlockNewPoint)
				{
					//set button invisible
					flagPassed->setVisible(false);
					//add old flag
					Sprite* oldLock = Sprite::create("UI/button/flag-n.png");
					oldLock->setPosition(vectorPoint.at(i));
					map->addChild(oldLock);
					//add eff 
					auto particle2 = ParticleSystemQuad::create("res/SmallSun.plist");
					particle2->setTotalParticles(100);
					particle2->setPosition(vectorPoint.at(i));
					particle2->setStartColor(Color4F(1, 1, 0, 1));
					particle2->setBlendAdditive(false);
					particle2->setVisible(false);
					particle2->setScale(0.1);
					map->addChild(particle2, 10);
					//add alert
					Alert* alert = new Alert("YOU GOT 3 MAGIC POINT", "", Alert::CONFIRM_SIZE);
					alert->getRightActionButton()->setVisible(false);
					alert->setVisible(false);
					alert->getLeftActionButton()->getButton()->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
					this->addChild(alert->getAlertLayer(), 11);
					//add point to pack
					PackConfig::getInstance()->addMagicPoint(3);
					alert->getAlertLayer()->runAction(
						Sequence::createWithTwoActions(
						DelayTime::create(2.4f), 
						CallFunc::create([&, alert](){alert->setVisible(true); })));
					//add listener for alert
					alert->getLeftActionButton()->getButton()->addClickEventListener([&, alert, this](Ref* event){
						alert->setVisible(false);
						this->setEnabled(true);
					});
					//run eff at next point
					auto setEff = CallFunc::create([&, oldLock, flagPassed, particle2](){
						oldLock->setVisible(false);
						flagPassed->setVisible(true);
						particle2->setVisible(true);
						flagPassed->runAction(FadeIn::create(0.5));
						particle2->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.7, 3), CallFunc::create([&, particle2](){particle2->setVisible(false); })));
					});

					Sequence* unlocEff = Sequence::createWithTwoActions(DelayTime::create(1.7f), setEff);
					oldLock->runAction(unlocEff);
					
				}
			}
			//set position for flag
			flagPassed->setPosition(vectorPoint.at(i));
			flagPassed->setTag(i + 1);
			flagPassed->addClickEventListener([&, flagPassed, this](Ref* event){
				if (flagPassed->getTag() <= 4)
					this->setEnabled(false);
				if (flagPassed->getTag() > 4)
				{
					this->toastAText("Sorry, this map still on development");
				}
				MapScene::runMap(flagPassed->getTag());
			});
			this->listMapPoint.push_back(flagPassed);
			map->addChild(flagPassed, 3);
		}
		else
		{
			//add next point level
			Sprite* nextFlag = Sprite::create("UI/button/flag-n.png");
			nextFlag->setPosition(vectorPoint.at(i));
			nextFlag->setTag(-1);
			map->addChild(nextFlag, 3);
		}
		
	}
	isUnlockNewPoint = false;
	log("DONE MAP");
}

void MapScene::setEnabled(bool isEnable)
{
	for (int i = 0; i < this->listMapPoint.size(); i++)
	{
		this->listMapPoint[i]->setEnabled(isEnable);
	}
	this->btnBack->setEnabled(isEnable);
}


void MapScene::toastAText(std::string mes)
{
	this->toastText->stopAllActions();
	this->toastText->setVisible(true);
	this->toastText->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->toastText->setOpacity(255);
	this->toastText->setString(mes);
	this->toastText->runAction(Spawn::createWithTwoActions(Repeat::create(MoveBy::create(1.5, Vec2(0, 150)), 1), Repeat::create(FadeOut::create(0.5), 1)));
}