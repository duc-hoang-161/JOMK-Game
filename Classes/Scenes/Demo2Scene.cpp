#include "Scenes\Demo2Scene.h"
#include "ui\UIButton.h"
#include"Scenes\SettingOverlay.h"
#include "Character\Items\Key.h"
#define SCALE_RATIO 32.0

Scene* Demo2Scene::createScene()
{
	auto scene = Scene::create();
#if SHOW_PHYSICS	

#endif

	// 'layer' is an autorelease object
	GameObjectManager::getInstance()->getGameHub()->removeMapLayer();
	auto layer = Demo2Scene::create();
	GameObjectManager::getInstance()->getGameHub()->setMapLayer(layer);

	// add layer as a child to scene
	scene->addChild(GameObjectManager::getInstance()->getGameHub()->getLayer());	

	// return the scene
	return scene;
}

void Demo2Scene::createMap()
{
	m_tileMap = GameObjectManager::getInstance()->getTileMap();
	addChild(m_tileMap);
	m_tileMap->release();
}

void Demo2Scene::createSetup()
{
	createMap();
	createCharacter();
	createMonsters();
	createTraps();
	createKeyAndDoor();
}

void Demo2Scene::createCharacter() 
{
	m_mainCharacter = GameObjectManager::getInstance()->getMainCharacter();		
	m_mainCharacter->addParent(this);
}

void Demo2Scene::createMonsters() 
{
	m_monsters = GameObjectManager::getInstance()->getMonsters();

	for (int i = 0; i < m_monsters.size(); i++)
		m_monsters[i]->addParent(this);
}

void Demo2Scene::createTraps()
{
	m_traps = GameObjectManager::getInstance()->getTraps();

	for (int i = 0; i < m_traps.size(); i++)
		m_traps[i]->addParent(this);
}

void Demo2Scene::createKeyAndDoor()
{
	m_keyMonster = GameObjectManager::getInstance()->getKeyMonster();
	m_keyMonster->addParent(this);

	m_door = GameObjectManager::getInstance()->getDoor();
	m_door->addParent(this);
}


bool Demo2Scene::init()
{
	Size screenSize = Director::getInstance()->getVisibleSize();
	if (!Layer::init())
		return false;
	AudioConfig::getInstance()->playTheme(1);
	m_physicsWorld = GameObjectManager::getInstance()->getPhysicsWorld();
	m_isUpdate = true;

	debugDraw = new (std::nothrow) GLESDebugDraw(32.0);
	//m_physicsWorld->SetDebugDraw(debugDraw);
	// m_physicsWorld->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit; // xem hình dạng
	debugDraw->SetFlags(flags);
	m_physicsWorld->SetAllowSleeping(false);
	PhysicsContactManager *pcm = new PhysicsContactManager();
	m_physicsWorld->SetContactListener(pcm);

	this->scheduleUpdate();

	/*auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Demo2Scene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Demo2Scene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	createSetup();	
	
	return true;
}

void Demo2Scene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	// // IMPORTANT: 
	// This is only for debug purposes 
	// It is recommend to disable it // 
	Layer::draw(renderer, transform, flags);
	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	_modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	//_customCommand.init(_globalZOrder);
	_customCommand.init(1);
	_customCommand.func = CC_CALLBACK_0(Demo2Scene::onDraw, this);
	renderer->addCommand(&_customCommand);

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void Demo2Scene::onDraw()
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");

	auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
	m_physicsWorld->DrawDebugData();
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}
void Demo2Scene::addWall(float w, float h, float px, float py) {

	b2PolygonShape floorShape; // Hình dạng Sàn

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO); // Hình vuông, hoặc chữ nhật
	b2FixtureDef floorFixture;

	floorFixture.density = 0;
	floorFixture.friction = 10;
	floorFixture.restitution = 0.5;
	floorFixture.shape = &floorShape;

	b2BodyDef floorBodyDef;
	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);

	b2Body *floorBody = m_physicsWorld->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixture);

}

void Demo2Scene::update(float deltaTime) {

	if (!m_isUpdate)
		return;

	int positionIterations = 3;  // Vị trí
	int velocityIterations = 8; // Vận tốc
	m_physicsWorld->Step(deltaTime, velocityIterations, positionIterations);

	// update hub

	for (int i = 0; i < m_monsters.size(); i++)
		m_monsters[i]->update(deltaTime);
	for (int i = 0; i < m_traps.size(); i++)
		m_traps[i]->update(deltaTime);
	if (m_keyMonster != NULL)
		m_keyMonster->update(deltaTime);
	if (m_door != NULL)
		m_door->update(deltaTime);

	// update character
	if (!m_mainCharacter->isAlive())
		return;

	bool isIDLE = true;
	/*if (m_key[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
		m_mainCharacter->moveRight(deltaTime), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
		m_mainCharacter->moveLeft(deltaTime), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_SPACE])
		m_mainCharacter->jumpUp(), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_X])
		m_mainCharacter->hitWithStick(), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_C])
		m_mainCharacter->runSkill("IRON_FIST"), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_V])
		m_mainCharacter->runSkill("RAGE_MODE"), isIDLE = false;*/

	m_mainCharacter->update(deltaTime);

	if (isIDLE)
		GameObjectManager::getInstance()->getGameHub()->update(deltaTime);

	setViewPointCenter(m_mainCharacter->getPosition());
}

void Demo2Scene::setViewPointCenter(Point pos) {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int x = MAX(pos.x, visibleSize.width / 2);
	int y = MAX(pos.y, visibleSize.height / 2);
	x = MIN(x, (m_tileMap->getMapSize().width* m_tileMap->getTileSize().width) - visibleSize.width / 2);
	y = MIN(y, (m_tileMap->getMapSize().height * m_tileMap->getTileSize().height) - visibleSize.height / 2);

	auto actualPos = ccp(x, y);
	auto centerOfView = ccp(visibleSize.width / 2, visibleSize.height / 2);
	auto viewPoint = ccpSub(centerOfView, actualPos);

	GameObjectManager::getInstance()->getGameHub()->setViewPointCenter(viewPoint);

	this->setPosition(viewPoint);
}

void Demo2Scene::stopUpdate()
{
	m_isUpdate = false;
}
//
//void Demo2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
//{
//	log("Key with keycode %d pressed", keyCode);
//
//	m_key[keyCode] = true;
//}
//
//void Demo2Scene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
//{
//	log("Key with keycode %d released", keyCode);
//
//	m_key[keyCode] = false;
//}