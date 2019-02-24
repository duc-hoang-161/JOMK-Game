#ifndef __DEMO_SCENE_H__
#define __DEMO_SCENE_H__

#include "cocos2d.h"
#include "json\document.h"
#include "Define.h"
#include "json\rapidjson.h"
#include "Character\Monsters\Monster.h"
#include "Character\MainCharacter\MainCharacter.h"
#include <string>
#include "Helper\Helper.h"
#include "GameManager\PhysicsContactManager.h"

using namespace cocos2d;
using namespace std;

class DemoScene : public cocos2d::Layer
{
private:
	//Map
	void setViewPointCenter(Point pos);
	TMXTiledMap *tileMap;
	void createMap();
	Layer *m_mapLayer;
	static string linkMap;
	static string linkAllMonster;
	//Event
	void DemoScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void DemoScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool m_isPress = false;
	map<EventKeyboard::KeyCode, bool> m_key;

	//Character
	MainCharacter *m_mainCharacter;
	vector<Monster*> m_monsters;
	void createCharacter();
	void createMonster();

	//Physics
	TMXLayer* m_physicsLayer;
	void createPhysics();

	bool onContactBegin(PhysicsContact& contact);
	void onContactEnd(PhysicsContact& contact);

	bool isCollision(PhysicsContact& contact, GameObject *gameObject, int BITMASK);
public:
	static cocos2d::Scene* createScene(string map, string monster);

	virtual bool init();
	void update(float deltaTime);

	CREATE_FUNC(DemoScene);

	void onHitWithStick();
};
	
#endif
