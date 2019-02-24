#ifndef COLLECTIONSCENE_H
#define COLLECTIONSCENE_H

#include "cocos2d.h"
#include "GUI\BaseUI.h"
#include "Scenes\MainScene.h"
#include "GUI\MyGridView.h"
#include "GUI\Slot.h"
#include "Import\PackConfig.h"
#include "Import\ItemsConfig.h"
#include "Scenes\ShopScene.h"
#include "ui\UIText.h"
#include "GUI\Alert.h"
#include "Import\AudioConfig.h"
using namespace cocos2d;

class CollectionScene :public BaseUI, public Scene
{
private:
	MyGridView *gridView;
	Button *btnEquip;
	void onItemSelected(Vec2 id, bool isSelected);
	void setDisplayDetail(std::string text, bool isVisible);
	void confirmSell();
	Alert* alert;
	int intentId;
	Slot* slotBody;
	void receiver(bool value);
	void setEnable(bool isEnable);
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CollectionScene);
	void setActionButton();
	void createPersonalLayout();
	void createAlertLayer();
	void onSlotBodySelected(int index, bool isSelected);
};

#endif