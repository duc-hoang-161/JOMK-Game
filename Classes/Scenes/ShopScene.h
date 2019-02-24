#ifndef SHOP_SCENE_H
#define SHOP_SCENE_H

#include "cocos2d.h"
#include "GUI\BaseUI.h"
#include "GUI\MyGridView.h"
#include "GUI\Slot.h"
#include "Import\PackConfig.h"
#include "Import\ItemsConfig.h"
#include "GUI\Alert.h"
#include "GUI\Button.h"
#include "Scenes\CollectionScene.h"
using namespace cocos2d;

class ShopScene : public BaseUI, public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ShopScene);
	MyGridView* gridView;
	Label* toastBuyItem;
	void toastNewItem(std::string mes);
	Alert* alert;
	Color3B color;
	void setActionButton();
	void onItemSelected(Vec2 id, bool isSelected);
	void receiver(bool value);
	int intentId;
	void setEnable(bool isEnable);
	void createAlertLayer();
	Button* btnSupport;
	Button* btnWeapon;
	Button* btnEquip;
	void createLayout();
	int typeItemShop;
};

#endif