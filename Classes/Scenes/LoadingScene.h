#include "cocos2d.h"
#include "ui\UILoadingBar.h"
#include "Scenes\MainScene.h"
#include "Import\AudioConfig.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

class LoadingScene : Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void createLayout();
	void initListener();
	bool onTouchBegan(Touch* touch, Event* event);
	Sprite* loadingBarBg;
	static ui::LoadingBar* loadingBar;
	static void setPercent(int percent);
	static Label* labelLoading;
	CREATE_FUNC(LoadingScene);
	virtual void update(float deltatime) override;
};

#endif