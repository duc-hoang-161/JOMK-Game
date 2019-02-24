#include "GameObjectManager.h"
#include "Scenes\LoadingScene.h"
#include "Scenes\MapScene.h"
#include "Scenes\LoadingMapScene.h"
#include "Scenes\Demo2Scene.h"

GameObjectManager* GameObjectManager::instance = NULL;

GameObjectManager::GameObjectManager()
{
	string content = FileUtils::getInstance()
		->getStringFromFile("loading_image.json");
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	m_keyMonster = 0;
	m_door = 0;
	m_physicsWorld = NULL;

	assert(document.IsArray());

	m_document = new rapidjson::Document();

	for (auto i = 0; i < document.GetArray().Size(); i++)
		m_images.push_back(document.GetArray()[i].GetString());

	m_index = 0;
	m_levelPlaying = -1;
	Director::getInstance()->getTextureCache()->addImageAsync(m_images[0], CC_CALLBACK_1(GameObjectManager::preload, this));	
}

void GameObjectManager::preload(Texture2D *texture)
{
	m_index++;
	if (m_index == m_images.size()) {
		createCoins();
		LoadingScene::setPercent(92);
		createMainCharacter();
		LoadingScene::setPercent(98);
		createGameHub();
		LoadingScene::setPercent(100);
	}
	else {
		LoadingScene::setPercent(m_index * 1.0 / m_images.size() * 90);
		Director::getInstance()->getTextureCache()->addImageAsync(m_images[m_index], CC_CALLBACK_1(GameObjectManager::preload, this));	
	}
}

void GameObjectManager::createCoins()
{
	m_coins.resize(15);
	for (int i = 0; i < m_coins.size(); i++)
		m_coins[i] = cocos2d::Sprite::create("opp-assets/UI/item/coin.png"),
		m_coins[i]->retain(),
		m_coins[i]->setVisible(false);
}

void GameObjectManager::createMainCharacter()
{
	m_mainCharacter = new MainCharacter("Characters/MainCharacter/MainCharacterAttribute.json",
		"Characters/MainCharacter/MainCharacterAnimation.json");
}

void GameObjectManager::createGameHub()
{
	m_gameHub = new GameHub();
}

cocos2d::Sprite* GameObjectManager::nextCoin()
{
	for (int i = 0; i < m_coins.size(); i++)
		if (!m_coins[i]->isVisible())
			return m_coins[i];

	return NULL;
}

void GameObjectManager::nextLevel()
{
	Director::getInstance()->replaceScene(
		MapScene::createScene(PackConfig::getInstance()->passNewPoint(this->m_levelPlaying)));
}

MainCharacter* GameObjectManager::getMainCharacter()
{
	return m_mainCharacter;
}

GameHub* GameObjectManager::getGameHub()
{
	return m_gameHub;
}

GameObjectManager* GameObjectManager::getInstance()
{
	if (GameObjectManager::instance == NULL)
		GameObjectManager::instance = new GameObjectManager();
	return GameObjectManager::instance;
}

void GameObjectManager::preloadGame(Texture2D *texture)
{
	m_gameIndex++;
	if (m_gameIndex == m_gameImages.size()) {
		createSetup();
	}
	else {
		LoadingMapScene::setPercent(m_gameIndex * 1.0 / m_gameImages.size() * 50);
		Director::getInstance()->getTextureCache()->addImageAsync(m_gameImages[m_gameIndex], CC_CALLBACK_1(GameObjectManager::preloadGame, this));
	}
}

void GameObjectManager::createNewGame(const std::string &file)
{
	destroyPreviousGame();

	string content = FileUtils::getInstance()
		->getStringFromFile(file);
	m_document->Parse<0>(content.c_str());

	m_gameIndex = 0;
	m_gameImages.clear();

	for (auto i = 0; i < (*m_document)["Loading"].GetArray().Size(); i++)
		m_gameImages.push_back((*m_document)["Loading"].GetArray()[i].GetString());

	m_gameIndex = 0;
	Director::getInstance()->getTextureCache()->addImageAsync(m_gameImages[0], CC_CALLBACK_1(GameObjectManager::preloadGame, this));
}

void GameObjectManager::startGame()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
		Director::getInstance()->replaceScene(Demo2Scene::createScene());
	});
}

void GameObjectManager::createSetup()
{
	createPhysicsWorld();
	LoadingMapScene::setPercent(50);

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]() {
		createMap();
		createPhysics();
		LoadingMapScene::setPercent(55);

		static rapidjson::Document *doc = m_document;

		if ((*doc).FindMember("Monsters") != (*doc).MemberEnd())
			this->createMonsters();
		LoadingMapScene::setPercent(60);

		if ((*doc).FindMember("Traps") != (*doc).MemberEnd())
			this->createTraps();
		LoadingMapScene::setPercent(80);

		if ((*doc).FindMember("NextScene") != (*doc).MemberEnd())
			this->createKeyAndDoor();

		LoadingMapScene::setPercent(100);
	});
}

void GameObjectManager::destroyPreviousGame()
{
	if (m_physicsWorld != NULL)
		delete m_physicsWorld;

	for (int i = 0; i < m_monsters.size(); i++)
		delete m_monsters[i];
	m_monsters.clear();

	for (int i = 0; i < m_traps.size(); i++)
		delete m_traps[i];
	m_traps.clear();

	if (m_keyMonster != NULL)
		delete m_keyMonster;

	if (m_door != NULL)
		delete m_door;
}

void GameObjectManager::createMap()
{
	m_tileMap = TMXTiledMap::create((*m_document)["Map"].GetString());
	m_tileMap->retain();

	auto physicsLayer = m_tileMap->getLayer("Physics");
	physicsLayer->setVisible(false);

	auto character = m_tileMap->getObjectGroup("Main")->getObject("Wukong");
	Vec2 position = ccp(character.at("x").asFloat(), character.at("y").asFloat());
	m_mainCharacter->setPosition(position);
}

void GameObjectManager::createPhysicsWorld()
{
	m_physicsWorld = new b2World(b2Vec2(0, -25.0f));

	m_mainCharacter->createPhysicsBody(m_physicsWorld);
}

void GameObjectManager::createPhysics()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto m_physicsLayer = m_tileMap->getLayer("Physics");
	Size layerSize = m_physicsLayer->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = m_physicsLayer->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				Vec2 pos = tileSet->getPosition();
				Size tileSize = tileSet->getContentSize();

				b2BodyDef myBodyDef;
				myBodyDef.type = b2_staticBody; //this will be a dynamic body
				myBodyDef.position.Set(
					b2Helper::asb2Value(pos.x),
					b2Helper::asb2Value(pos.y + 3.0 / 4 * tileSize.height));
				//set the starting position
				myBodyDef.angle = 0; //set the starting angle

				b2Body* dynamicBody = m_physicsWorld->CreateBody(&myBodyDef);
				b2PolygonShape boxShape;
				boxShape.SetAsBox(
					b2Helper::asb2Value(tileSize.width / 2),
					b2Helper::asb2Value(tileSize.height / 2));
				b2FixtureDef boxFixtureDef;
				boxFixtureDef.shape = &boxShape;
				boxFixtureDef.friction = 1;
				boxFixtureDef.filter.categoryBits = BITMASK_GROUND;
				boxFixtureDef.filter.maskBits = -1;

				dynamicBody->CreateFixture(&boxFixtureDef);
			}
		}
	}
}

void GameObjectManager::createMonsters()
{
	auto monsters = (*m_document)["Monsters"].GetArray();
	for (rapidjson::SizeType i = 0; i < monsters.Size(); i++) {
		const rapidjson::Value &monster = monsters[i];

		string objectGroup = monster["ObjectGroup"].GetString();
		string type = monster["Type"].GetString();

		auto objects = monster["Object"].GetArray();
		for (rapidjson::SizeType j = 0; j < objects.Size(); j++) {
			string object = objects[j].GetString();

			Monster *m = Helper::createMonsterByType(type);
			if (monster.FindMember("Attribute") != monster.MemberEnd())
				m->setAttribute(&monster["Attribute"]);

			auto inMap = m_tileMap->getObjectGroup(objectGroup)->getObject(object);
			Vec2 position = ccp(inMap.at("x").asFloat(), inMap.at("y").asFloat());
			m->setPosition(position);
			m->createPhysicsBody(m_physicsWorld);
			m->addMainCharacter(m_mainCharacter);

			m_monsters.push_back(m);
		}
	}
}

void GameObjectManager::createTraps()
{
	auto traps = (*m_document)["Traps"].GetArray();

	for (rapidjson::SizeType i = 0; i < traps.Size(); i++) {
		const rapidjson::Value &trap = traps[i];

		string objectGroup = trap["ObjectGroup"].GetString();
		string type = trap["Type"].GetString();

		auto objects = trap["Object"].GetArray();
		for (rapidjson::SizeType j = 0; j < objects.Size(); j++) {
			string object = objects[j].GetString();

			Trap *t = Helper::createTrapByType(type);
			t->setAttribute(&trap["Attribute"]);

			auto inMap = m_tileMap->getObjectGroup(objectGroup)->getObject(object);
			Vec2 position = ccp(inMap.at("x").asFloat(), inMap.at("y").asFloat());
			t->setPosition(position);
			t->createPhysicsBody(m_physicsWorld);
			t->addMainCharacter(m_mainCharacter);

			m_traps.push_back(t);
		}
	}
}

void GameObjectManager::createKeyAndDoor()
{
	auto keyAndDoor = &(*m_document)["NextScene"];

	if (keyAndDoor->FindMember("Key") != keyAndDoor->MemberEnd()) {
		const rapidjson::Value &key = (*keyAndDoor)["Key"];

		m_keyMonster = Helper::createMonsterByType("BoxMonster");
		if (key.FindMember("Attribute") != key.MemberEnd())
			m_keyMonster->setAttribute(&key["Attribute"]);

		std::string objectGroup = key["ObjectGroup"].GetString();
		std::string object = key["Object"].GetString();
		auto inMap = m_tileMap->getObjectGroup(objectGroup)->getObject(object);
		Vec2 position = ccp(inMap.at("x").asFloat(), inMap.at("y").asFloat());

		m_keyMonster->setPosition(position);
		m_keyMonster->createPhysicsBody(m_physicsWorld);
		m_keyMonster->addMainCharacter(m_mainCharacter);

		Key *item = new Key();
		item->createPhysicsBody(m_physicsWorld);
		m_keyMonster->addItem(item);

		m_keyMonster->getPhysicsBody()->SetType(b2_staticBody);
	}

	if (keyAndDoor->FindMember("Door") != keyAndDoor->MemberEnd()) {
		const rapidjson::Value &door = (*keyAndDoor)["Door"];
		if (door.FindMember("Direct") != door.MemberEnd()) {
			std::string file = door["Direct"].GetString();
			m_door = new Door(file);

			std::string objectGroup = door["ObjectGroup"].GetString();
			std::string object = door["Object"].GetString();
			auto inMap = m_tileMap->getObjectGroup(objectGroup)->getObject(object);
			Vec2 position = ccp(inMap.at("x").asFloat(), inMap.at("y").asFloat());

			m_door->setPosition(position);
			m_door->createPhysicsBody(m_physicsWorld);

			m_door->getPhysicsBody()->SetType(b2_staticBody);
		}

	}
}

b2World* GameObjectManager::getPhysicsWorld()
{
	return m_physicsWorld;
}

cocos2d::TMXTiledMap* GameObjectManager::getTileMap()
{
	return m_tileMap;
}

std::vector<Monster*> GameObjectManager::getMonsters()
{
	return m_monsters;
}

std::vector<Trap*> GameObjectManager::getTraps()
{
	return m_traps;
}

Monster* GameObjectManager::getKeyMonster()
{
	return m_keyMonster;
}

Door* GameObjectManager::getDoor()
{
	return m_door;
}

void GameObjectManager::destroyInstance()
{
	if (GameObjectManager::instance != NULL)
		delete GameObjectManager::instance;
}

GameObjectManager::~GameObjectManager()
{
	for (int i = 0; i < m_coins.size(); i++)
		m_coins[i]->release();

	delete m_mainCharacter;
	delete m_gameHub;

	destroyPreviousGame();
}

void GameObjectManager::setCurrentPlayLevel(int level)
{
	this->m_levelPlaying = level;
}