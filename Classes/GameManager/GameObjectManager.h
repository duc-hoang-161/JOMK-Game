#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include <iostream>
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Character\MainCharacter\MainCharacter.h"
#include "GameManager\GameHub.h"
#include "Character\Items\Key.h"
#include "Character\Door\Door.h"

class Door;

class GameObjectManager {
private:
	static GameObjectManager *instance;
	GameObjectManager();

		// preload
	std::vector<string> m_images;
	int m_index;
	void preload(Texture2D *texture);

		// preload game
	std::vector<std::string> m_gameImages;
	int m_gameIndex;
	void preloadGame(Texture2D *texture);

	std::vector<cocos2d::Sprite*> m_coins;
	MainCharacter *m_mainCharacter;
	int m_levelPlaying;
	GameHub *m_gameHub;

	rapidjson::Document *m_document;
	cocos2d::TMXTiledMap *m_tileMap;
	b2World *m_physicsWorld;
	vector<Monster*> m_monsters;
	vector<Trap*> m_traps;
	Monster *m_keyMonster;
	Door *m_door;

	void createSetup();
	void createPhysicsWorld();
	void createMap();
	void createPhysics();
	void createMonsters();
	void createTraps();
	void createKeyAndDoor();
	void destroyPreviousGame();
public:

	void createCoins();
	void createMainCharacter();
	void createGameHub();
	void setCurrentPlayLevel(int level);

	void createNewGame(const std::string &file);
	void startGame();

	b2World *getPhysicsWorld();
	cocos2d::TMXTiledMap* getTileMap();
	std::vector<Monster*> getMonsters();
	std::vector<Trap*> getTraps();
	Monster* getKeyMonster();
	Door* getDoor();

	cocos2d::Sprite* nextCoin();
	MainCharacter* getMainCharacter();
	GameHub *getGameHub();

	static GameObjectManager* getInstance();
	static void destroyInstance();
	~GameObjectManager();

	void nextLevel();
};

#endif