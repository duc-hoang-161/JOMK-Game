#ifndef __LOADING_MAP_SCENE_H__
#define __LOADING_MAP_SCENE_H__

#include <iostream>
#include "cocos2d.h"
#include "ui\UILoadingBar.h"
#include "Scenes\MainScene.h"
#include "Import\AudioConfig.h"
#include "SimpleAudioEngine.h"

class LoadingMapScene : Layer
{
private:
	static bool m_isChange;
public:
	static std::string m_fileDirect;
	static Scene* createScene(std::string file);
	virtual bool init();
	void createLayout();
	void initListener();
	bool onTouchBegan(Touch* touch, Event* event);
	Sprite* loadingBarBg;
	static ui::LoadingBar* loadingBar;
	static void setPercent(int percent);
	static Label* labelLoading;
	CREATE_FUNC(LoadingMapScene);
	virtual void update(float deltatime) override;
};

#endif