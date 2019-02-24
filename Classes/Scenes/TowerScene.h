#include "cocos2d.h"
#include "GUI\BaseUI.h"
#include "Items\MagicItem.h"
#include "GUI\Slot.h"
#include "Import\ItemsConfig.h"
#include "Import\PackConfig.h"
#include "ui\UIScrollView.h"
#include "GUI\Alert.h"
#include "Import\AudioConfig.h"
#include "GameManager\GameObjectManager.h"
using namespace cocos2d;

#ifndef TOWER_SCENE_H
#define TOWER_SCENE_H

class TowerScene : public BaseUI, public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TowerScene);
	Layer* createSkillTree(std::vector<MagicItem*> skills, int row);
	Layer* skillTreeLayer;
	ui::ScrollView* scrollSkill;
	void createLayout();
	void onItemClicked(int id, bool value);
	std::vector<std::vector<Slot*>> slotList;
	void setUpgrade(Vec2 id, int row);
	Label* toastText;
	void toastAText(std::string mes);
	Alert* alert;
	void setActionButton();
	std::vector<LayerColor*> lineSkillLayer;
	int selectedRow;
	int selectedCol;
	int intentId = 0;
	void receiver(bool value);
	void createPersonalLayout();
	Slot* slotBody;
};

#endif