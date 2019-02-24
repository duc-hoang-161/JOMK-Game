#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "ui/UIButton.h"
using namespace cocos2d;

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

class MainScene : Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void createLayout();
	ui::Button* btnShop;
	ui::Button* btnTower;
	ui::Button* btnArena;
	ui::Button* btnSmithy;
	ui::Button* btnPack;
	ui::Button* btnSetting;
	ui::Button* btnExit;
	Layer* settingOverlay;
	static int state;
	bool isMapRunning;
	void openMap();
	void setBuidlingButtonDisplay(bool visible);
	void addListener();
	CREATE_FUNC(MainScene);
};

#endif