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
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "GameManager\GameObjectManager.h"
#include "Helper\b2Helper.h"
#include"Character\Traps\Trap.h"
#include"Scenes\SettingOverlay.h"
#include "Character\Monsters\BatMonster.h"
#include "GameManager\GameHub.h"
#include"Character\Traps\TrapMoveDown.h"
#include "Scenes\MapScene.h"
#include "Character\Door\Door.h"

using namespace cocos2d;
using namespace std;

class Demo2Scene : public cocos2d::Layer
{
private:
	// box 2d
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;
	virtual void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags) override;
	void onDraw();
	void addWall(float w, float h, float px, float py);

	rapidjson::Document document;

	//Map
	void setViewPointCenter(Point pos);
	
	// MainCharacter
	MainCharacter *m_mainCharacter;
	b2World *m_physicsWorld;
	TMXTiledMap *m_tileMap;
	vector<Monster*> m_monsters;
	vector<Trap*> m_traps;
	Monster *m_keyMonster;
	Door *m_door;

	//Setup
	void createSetup();
	void createCharacter();
	void createMap();
	void createMonsters();
	void createTraps();
	void createKeyAndDoor();

	//////Event
	//void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	//map<EventKeyboard::KeyCode, bool> m_key;

	bool m_isUpdate;
public:
	void stopUpdate();

	static cocos2d::Scene* createScene();

	virtual bool init();
	void update(float deltaTime);

	CREATE_FUNC(Demo2Scene);
};

#endif
