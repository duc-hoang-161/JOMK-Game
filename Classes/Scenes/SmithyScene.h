#include "cocos2d.h"
#include "GUI\BaseUI.h"
#include "GUI\MyGridView.h"
#include "GUI\Slot.h"
#include "GUI\Alert.h"
#include "Import\ItemsConfig.h"
#include "Import\PackConfig.h"
using namespace cocos2d;

#ifndef SMITHY_SCENE_H
#define SMITHY_SCENE_H

class SmithyScene : public BaseUI, public Scene
{
private:
	MyGridView* gridViewPiece;
	MyGridView* gridViewUpgradeItems;
	void createLayout();
	void createGridView();
	Slot* slotUpgrade;
	Slot* slotPiece;
	Slot* slotNextItem;
	void onRockItemSelected(Vec2 id, bool isSelected);
	void onUpgradeItemSelected(Vec2 id, bool isSelected);
	void onSlotUpgradeSelected(Vec2 indexSlot, bool isSelected);
	int indexRock;
	std::vector<Vec3> listItem;
	int indexUpgrade;
	Label* toastText;
	Label* costUpgrade;
	Label* percentUpgrade;
	bool getResultUpgrade(int value);
	void toastAText(std::string mes);
	void showUpgradeResult(int mode);
	void readyToUpgrade();
	void showInfo(std::string info, bool isVisible);
	void upgradeItem();
	Alert* alert;
	int intentId;
	void receiver(bool value);
	Vector<SpriteFrame*> animationBuild;
	Sprite* buildNew;
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SmithyScene);
};

#endif