#include "cocos2d.h"
#include "ui/UIButton.h"
#include "GUI\Button.h"
#include "MainScene.h"
#include "GUI\Alert.h"
#include "ui/UICheckBox.h"
#include "ui/UISlider.h"
using namespace cocos2d;

#ifndef SETTING_OVERLAY_H
#define SETTING_OVERLAY_H

class SettingOverlay
{
private:
	//SettingOverlay* settingOject;
	static Layer* settingOverlay;
	void createLayout();
	Layer* layerMenu;
	Button* btnReturnMain;
	Button* btnAudioSetting;
	Button* btnTutorial;
	Button* btnAbout;
	Button* btnBack;
	Alert* setting;
	Alert* tutorial;
	Alert* about;
	void addListener();
	std::function<void(bool)> callback;
	static int TYPE;
	virtual void changeLayout(int mode);
	void createAudioSettingLayout();
public:
	SettingOverlay();
	SettingOverlay(int mode);
	static ui::Button* getSettingButton();
	static SettingOverlay* getInstance(int mode);
	void init();
	void setCallBack(std::function<void(bool)> callback);
	Layer* getOverlay();
	void setEnable(bool isEnable);
	ui::Button* btnSetting;
	static const int FULL_OPERATION = 1;
	static const int LITE_OPERATION = 2;
	
	void close();

	~SettingOverlay();
};

#endif