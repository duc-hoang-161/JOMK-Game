#ifndef __GAME_HUB_H__
#define __GAME_HUB_H__

#include "cocos2d.h"
#include"Scenes\SettingOverlay.h"
#include "GUI\SneakyJoystick.h"
#include "GUI\SneakyJoystickSkinnedBase.h"
#include "Import\ItemsConfig.h"
class GameHub{
public:
	static const int BUTTON_LEFT;
	static const int BUTTON_RIGHT;
	static const int BUTTON_FIGHT;
	static const int BUTTON_JUMP;
	static const int BUTTON_FIRST_SKILL;
	static const int BUTTON_SECOND_SKILL;

	static const int WIN;
	static const int LOSE;

private:
	Vec2 m_pointCenter;
	ui::Button* btnFight;
	ui::Button* btnJump;
	ui::Button* btnFirstSkill;
	ui::Button* btnSecSkill;
	ui::Button* bluePotion;
	ui::Button* redPotion;
	Sprite *m_keyImage;
	SettingOverlay *m_settingOverlay;
	std::map<int, bool> m_touchID;

	Sprite *m_healthBarBackground;
	ui::LoadingBar *m_healthBar;
	Sprite *m_powerBarBackground;
	ui::LoadingBar *m_powerBar;
	Sprite *m_energyBarBackground;
	ui::LoadingBar *m_energyBar;
	Label *m_coins;
	Label *slotAvailable;
	void createControlButton();
	void createHealthAndPowerBar();
	void createCoins();
	bool isPress(int buttonID);
	cocos2d::Layer *m_hubLayer;
	cocos2d::Layer *m_mapLayer;
	std::string srcFirst;
	std::string srcSec;

	Label* timeFirstSkill;
	Label* timeSecondSkill;
public:
	GameHub();

	void countDownLabel(Label *label, int time);

	void setSkillButton();
	void setEnable(bool isEnable);
	cocos2d::Layer* getLayer();

	void setMapLayer(Layer *layer);
	void removeMapLayer();
	void update(float deltaTime);
	void showAlert(int MODE);
	SneakyJoystick *leftJoystick;
	void setViewPointCenter(Vec2 pos);

	void setVisibleKey(bool v);
	bool isHaveKey();
		// move sprite coins to m_coins position
	void moveToCoins(Sprite *sprite, Vec2 positionInMap);
		// move sprite (item, ...) to bag
	void moveToBag(Sprite *sprite, Vec2 positionInMap);

	void setButtonTouch(int key, bool value);
	~GameHub();
	void setSkillButtonAction(ui::Button* btn, int idSkill, bool isFirst = true);
private:
	bool m_startEnergyBar;
	int t_startedEnergyBarTime;
public:
	void startEnergyBar();
	void stopEnergyBar();
};

#endif