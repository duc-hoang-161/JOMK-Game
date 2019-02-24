#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Define.h"
#include "cocos2d.h"

#include "Objects\LivingObject.h"
#include "Objects\DynamicGameObject.h"
#include "Character\Bullet\SuperBullets.h"
#include "Helper\b2Helper.h"
#include "Character\Skill\Skill.h"
#include "Import\AudioConfig.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

class Skill;

class MainCharacter : public DynamicGameObject, public LivingObject {
private:
	static const int STATE_IDLE;
	static const int STATE_WALK;
	static const int STATE_JUMP;
	static const int STATE_DIE;
public:
	static const int STATE_HIT;
private:
	static const int STATE_HIT_WITH_STICK;
	static const int STATE_GETHIT;
	static const int STATE_FLY;
	static const int STATE_FLY_GETHIT;
	static const int STATE_FLY_ON_STICK;
	static const int STATE_FLY_ON_STICK_GETHIT;

	int m_runStep;
	int m_HP;
	int m_maxHP;
	int m_armor;
	int m_energy;
	int m_maxEnergy;
	int m_magic;
	int m_energyPerSecond;
	int m_hitWithStickDamage;

	int m_basicHitWithStickDamage;
	int m_basicArmor;

	int nContactWithGround = 0;

	SuperBullets *m_bullets, *m_powerBullets;
	
	bool m_isPowerState;
	ParticleFire *m_particleFire;
	int m_currentBulletType;
	void createBullet();

private:
	std::map<std::string, Skill*> m_skills;
	void createSkill();
public:
	bool isRuningSkill(std::string name);
	void runSkill(std::string name);
	void setLevelSkill(std::string name, int level);
	int getTimeOfSkill(std::string name);

public:
	void setCurrentBulletType(int type);
	static const int BULLET_SMALL;
	static const int BULLET_MEDIUM;
	static const int BULLET_BIG;

	static const int REGION_BODY;
	static const int REGION_DAMAGE;


	MainCharacter(string attributeFileDirect, string animationFileDirect);

	void initialize();

	// override method of GameObject class
	void update(float deltaTime);
	string className() { return "MC"; }
	void addParent(Layer *parent);
	void createPhysicsBody(b2World *physicsWorld);

	//get,set
	int getHP();
	int getMaxHP();
	int getMagic();
	int getEnergy();
	int getMaxEnergy();
	int getHitWithStickDamage();
	void addHitWithStickDamage(int number);
	void addHP(int number);
	void addEnergy(int number);
	void addRunStep(int number);
	void subEnergy(int number);
	void setRunStep(int number);
	void setPowerState(bool v);
	int getRunStep();
	// New method
	void startParticleFire();
	void stopParticleFire();
	void idle();
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void jumpUp();
	void nextBullet();
	void hitWithStick();
	void addContactWithGround(int v);
	void die();
	void gethit();

	void attack(LivingObject *lObj);
	
	~MainCharacter();
};

#endif