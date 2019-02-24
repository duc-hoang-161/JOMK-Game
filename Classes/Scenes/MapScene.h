#include "cocos2d.h"
#include "GUI\Button.h"
#include "Import\PackConfig.h"
#include "ui\UIScrollView.h"
using namespace cocos2d;

#ifndef MAP_SCENE_H
#define MAP_SCENE_H

class MapScene : public Scene
{
private:
	std::vector<ui::Button*> listMapPoint;
public:
	static Scene* createScene();
	ui::ScrollView* scrollMap;
	static Scene* createScene(bool isUnlock);
	virtual bool init();
	CREATE_FUNC(MapScene);
	Layer* mapLayer;
	Label* toastText;
	void toastAText(std::string mes);
	Sprite* map;
	Button* btnBack;
	void createMapScene();
	static void runMap(int mapLevel);
	void addPointLevel();
	void setEnabled(bool isEnable);
};

#endif