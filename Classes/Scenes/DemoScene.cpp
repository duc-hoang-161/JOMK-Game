#include "Scenes\DemoScene.h"
#include "ui\UIButton.h"
string DemoScene::linkMap = "";
string DemoScene::linkAllMonster = "";
Scene* DemoScene::createScene(string map,string monster)
{
	linkMap = map;
	linkAllMonster = monster;
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	
	auto pw = scene->getPhysicsWorld();
#if SHOW_PHYSICS	
	
#endif

	// 'layer' is an autorelease object
	auto layer = DemoScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


void DemoScene::createMap()
{
	m_mapLayer = Layer::create();
	tileMap = TMXTiledMap::create(linkMap);
	m_physicsLayer = tileMap->getLayer("Physics");
	m_physicsLayer->setVisible(false);
	m_mapLayer->addChild(tileMap);
	addChild(m_mapLayer);
	 
	
		// demo button
	auto sS = Director::getInstance()->getVisibleSize();
	ui::Button *btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	btn->setPosition(Vec2(0.2 * sS.width, 0.2 * sS.height));
	this->addChild(btn);
}

void DemoScene::createCharacter() {
	m_mainCharacter = new MainCharacter("Characters/MainCharacter/MainCharacterAttribute.json",
		"Characters/MainCharacter/MainCharacterAnimation.json");
	auto character = tileMap->getObjectGroup("Main")->getObject("Wukong");
	Vec2 position = ccp(character.at("x").asFloat(), character.at("y").asFloat());
	m_mainCharacter->setPosition(position);
	m_mainCharacter->addParent(m_mapLayer);
}

void DemoScene::createMonster() {
	string content = FileUtils::getInstance()->getStringFromFile(linkAllMonster);
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	auto monsters = document["Monsters"].GetArray();
	for (rapidjson::SizeType i = 0; i < monsters.Size(); i++) {
		const rapidjson::Value &monster = monsters[i];
		string objectGroup = monster["ObjectGroup"].GetString();
		string object = monster["Object"].GetString();
		
		string name = monster["Type"].GetString();
		string animation = monster["Animation"].GetString();
		string attribute = monster["Attribute"].GetString();

		Monster *result = Helper::createMonsterByType(name);
		m_monsters.push_back(result);

		auto inMap = tileMap->getObjectGroup(objectGroup)->getObject(object);
		Vec2 position = ccp(inMap.at("x").asFloat(), inMap.at("y").asFloat());
		result->setPosition(position);
		result->addParent(m_mapLayer);
	}
}

bool DemoScene::init() 
{
	if (!Layer::init())
		return false;

	Size screenSize = Director::getInstance()->getVisibleSize();
	this->scheduleUpdate();
	createMap();
	createPhysics();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactSeparate = CC_CALLBACK_1(DemoScene::onContactEnd, this);
	contactListener->onContactBegin = CC_CALLBACK_1(DemoScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(DemoScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(DemoScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	createCharacter();
	createMonster();

	return true;
}

void DemoScene::createPhysics() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//world
	/*auto edgeBody = PhysicsBody::createEdgeBox(visibleSize + Size(0, 200), PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->setCollisionBitmask(BITMASK_WORLD);
	edgeBody->setContactTestBitmask(true);

	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);*/

	Size layerSize = m_physicsLayer->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = m_physicsLayer->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				auto physics = PhysicsBody::createBox(tileSet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
				physics->setCollisionBitmask(BITMASK_GROUND);
				physics->setContactTestBitmask(true);
				physics->setDynamic(false);
				physics->setResting(false);
				physics->setMass(50);
				tileSet->setPhysicsBody(physics);
			}
		}
	}
}

void DemoScene::update(float deltaTime) {
	m_mainCharacter->update(deltaTime);
	for (int i = 0; i < m_monsters.size(); i++)
		m_monsters[i]->update(deltaTime);


	bool isIDLE = true;

	if (m_key[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
		m_mainCharacter->moveRight(deltaTime), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
		m_mainCharacter->moveLeft(deltaTime), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_SPACE])
		m_mainCharacter->jumpUp(), isIDLE = false;

	if (m_key[EventKeyboard::KeyCode::KEY_X]) {
		CallFunc *func = CallFunc::create([this]() {
		});
		m_mainCharacter->hitWithStick(), isIDLE = false;
	}

	if (m_key[EventKeyboard::KeyCode::KEY_C]) {
		CallFunc *func = CallFunc::create([this]() {
		});
	}

	if (isIDLE)
		m_mainCharacter->idle();
	setViewPointCenter(m_mainCharacter->getPosition());
}

void DemoScene::setViewPointCenter(Point pos) {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int x = MAX(pos.x, visibleSize.width / 2);
	int y = MAX(pos.y, visibleSize.height / 2);
	x = MIN(x, (tileMap->getMapSize().width* tileMap->getTileSize().width) - visibleSize.width / 2);
	y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) - visibleSize.height / 2);

	auto actualPos = ccp(x, y);
	auto centerOfView = ccp(visibleSize.width / 2, visibleSize.height / 2);
	auto viewPoint = ccpSub(centerOfView, actualPos);
	
	m_mapLayer->setPosition(viewPoint);
}


void DemoScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d pressed", keyCode);

	m_key[keyCode] = true;
}

void DemoScene::onHitWithStick() {
}

void DemoScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d released", keyCode);

	m_key[keyCode] = false;
}

bool DemoScene::onContactBegin(PhysicsContact& contact)
{
 
	return true;
}	

void DemoScene::onContactEnd(PhysicsContact& contact)
{
	if (isCollision(contact, m_mainCharacter, BITMASK_GROUND))
		m_mainCharacter->addContactWithGround(-1);
}

bool DemoScene::isCollision(PhysicsContact& contact, GameObject *gameObject, int BITMASK)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	contact.getShapeA()->setRestitution(0.4);

	if ((a->getCollisionBitmask() == BITMASK && b->getCollisionBitmask() == gameObject->getTag())
		|| (a->getCollisionBitmask() == gameObject->getTag() && b->getCollisionBitmask() == BITMASK))
		return true;
	return false;
}