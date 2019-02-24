#include "MainCharacter.h"

const int MainCharacter::REGION_BODY = 1;
const int MainCharacter::REGION_DAMAGE = 2;

const int MainCharacter::STATE_IDLE = 1;
const int MainCharacter::STATE_WALK = 2;
const int MainCharacter::STATE_JUMP = 3;
const int MainCharacter::STATE_DIE = 4;
const int MainCharacter::STATE_HIT = 5;
const int MainCharacter::STATE_HIT_WITH_STICK = 6;
const int MainCharacter::STATE_GETHIT = 7;
const int MainCharacter::STATE_FLY = 8;
const int MainCharacter::STATE_FLY_GETHIT = 9;
const int MainCharacter::STATE_FLY_ON_STICK = 10;
const int MainCharacter::STATE_FLY_ON_STICK_GETHIT = 11;

const int MainCharacter::BULLET_SMALL = 1;
const int MainCharacter::BULLET_MEDIUM = 2;
const int MainCharacter::BULLET_BIG = 3;

#include "GameManager\GameObjectManager.h"

MainCharacter::MainCharacter(string attributeFileDirect, string animationFileDirect) :
	DynamicGameObject(animationFileDirect)
{
	m_sprite->setAnchorPoint(Vec2(0.5, 0));

	pRegisterPhysicsBody(&m_physicsBody);
	pRegisterHP(&m_HP);
	pRegisterMaxHP(&m_maxHP);
	pRegisterArmor(&m_armor);
	pRegisterRunStep(&m_runStep);
	pRegisterSprite(&m_sprite);

	// set action priority for mc
	setActionPriority(MainCharacter::STATE_IDLE, 1);
	setActionPriority(MainCharacter::STATE_FLY, 1);
	setActionPriority(MainCharacter::STATE_FLY_ON_STICK, 1);
	setActionPriority(MainCharacter::STATE_WALK, 1);
	setActionPriority(MainCharacter::STATE_GETHIT, 2);
	setActionPriority(MainCharacter::STATE_FLY_GETHIT, 2);
	setActionPriority(MainCharacter::STATE_FLY_ON_STICK_GETHIT, 2);
	setActionPriority(MainCharacter::STATE_JUMP, 3);
	setActionPriority(MainCharacter::STATE_HIT_WITH_STICK, 3);
	setActionPriority(MainCharacter::STATE_HIT, 3);
	setActionPriority(MainCharacter::STATE_DIE, 3);
	
	// set filterData for mc
	m_filterData.categoryBits = BITMASK_CHARACTER;
	m_filterData.maskBits = -1;

	if (m_physicsBodyManager != NULL) {
		b2Filter filterBody;
		filterBody.categoryBits = BITMASK_CHARACTER;
		filterBody.maskBits = -1;
		m_physicsBodyManager->addFilter(REGION_BODY, filterBody);

		b2Filter filterDamage;
		filterDamage.categoryBits = BITMASK_CHARACTER;
		filterDamage.maskBits = BITMASK_MONSTER;
		m_physicsBodyManager->addFilter(REGION_DAMAGE, filterDamage);
	}

	// read file contains attribute of mc
		// parse JSON file
	string content = FileUtils::getInstance()
		->getStringFromFile(attributeFileDirect);
	rapidjson::Document document;
	document.Parse<0>(content.c_str());
		// read attribute
	m_runStep = document["RunStep"].GetInt();
	m_HP = m_maxHP = document["MaxHP"].GetInt();
	m_energy = m_maxEnergy = document["MaxEnergy"].GetInt();
	m_magic = document["Magic"].GetInt();
	m_basicHitWithStickDamage = document["HitWithStickDamage"].GetInt();
	m_basicArmor = document["Armor"].GetInt();
	m_energyPerSecond = document["EnergyPerSecond"].GetInt();

	createBullet();

	m_isPowerState = false;
	m_particleFire = ParticleFire::create();
	m_particleFire->stop();
	m_particleFire->retain();

	static std::function<void(void)> addEnergy = [&]() {
		m_energy = std::min(m_energy + m_energyPerSecond, m_maxEnergy);
		Helper::waitAndExecute(1000, addEnergy);
	};
	addEnergy();

	createSkill();
}

int MainCharacter::getHitWithStickDamage()
{
	return m_hitWithStickDamage;
}

#include "Character\Skill\Flash.h"
#include "Character\Skill\Heal.h"
#include "Character\Skill\IronFist.h"
#include "Character\Skill\RageMode.h"

void MainCharacter::createSkill()
{
	string content = FileUtils::getInstance()
		->getStringFromFile("Characters/MainCharacter/Skills.json");
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	m_skills["FLASH"] = new Flash(&document["FLASH"]);
	m_skills["HEAL"] = new Heal(&document["HEAL"]);
	m_skills["IRON_FIST"] = new IronFist(&document["IRON_FIST"], m_bullets, m_powerBullets);
	m_skills["RAGE_MODE"] = new RageMode(&document["RAGE_MODE"]);

	for (auto it = m_skills.begin(); it != m_skills.end(); ++it)
		it->second->setMainCharacter(this);
}

#include "Import\PackConfig.h"
void MainCharacter::initialize()
{
	m_HP = m_maxHP;
	m_energy = m_maxEnergy;
	nContactWithGround = 0;
	m_sprite->setVisible(true);
	m_isAlive = true;
	m_sprite->setFlippedX(false);
	m_isPowerState = false;
	m_particleFire->stop();

	m_hitWithStickDamage = m_basicHitWithStickDamage;
	m_armor = m_basicArmor;

	for (int i = 0; i < 5; i++) {
		GameItem *gi = PackConfig::getInstance()->getEquipItemAt(i);
		if (gi == NULL)
			continue;
		Weapon *w;
		EquipItem *e;
		if ((w = dynamic_cast<Weapon*>(gi)) != NULL)
			m_hitWithStickDamage += w->getDamage();
		else if ((e = dynamic_cast<EquipItem*>(gi)) != NULL)
			m_armor += e->getDefense(),
			m_hitWithStickDamage += e->getDamage();
	}
}

void MainCharacter::runSkill(std::string name)
{
	m_skills[name]->execute();
}

void MainCharacter::setLevelSkill(std::string name, int level)
{
	m_skills[name]->setLevel(level);
}

void MainCharacter::createBullet()
{
	// normal bullet
	m_bullets = new SuperBullets();

	// bullet for first skill
	b2Filter filter;
	filter.categoryBits = BITMASK_BULLET;
	filter.maskBits = BITMASK_MONSTER;
	
	// small
	Bullets *m_small = new Bullets(3, "Characters/Bullet/Bullet03_small/BulletAnimation.json");
	m_small->setFilterData(filter);
	m_bullets->addBullets(BULLET_SMALL, m_small);

	// medium
	Bullets *m_medium = new Bullets(3, "Characters/Bullet/Bullet03_medium/BulletAnimation.json");
	m_medium->setFilterData(filter);
	m_bullets->addBullets(BULLET_MEDIUM, m_medium);

	// big
	Bullets *m_big = new Bullets(3, "Characters/Bullet/Bullet03_big/BulletAnimation.json");
	m_big->setFilterData(filter);
	m_bullets->addBullets(BULLET_BIG, m_big);

	// power bullets
	m_powerBullets = new SuperBullets();
	// small
	Bullets *m_powerSmall = new Bullets(3, "Characters/Bullet/Bullet09_small/BulletAnimation.json");
	m_powerSmall->setFilterData(filter);
	m_powerSmall->setAnimateForever(true);
	m_powerBullets->addBullets(BULLET_SMALL, m_powerSmall);

	// medium
	Bullets *m_powerMedium = new Bullets(3, "Characters/Bullet/Bullet09_medium/BulletAnimation.json");
	m_powerMedium->setFilterData(filter);
	m_powerMedium->setAnimateForever(true);
	m_powerBullets->addBullets(BULLET_MEDIUM, m_powerMedium);

	// big
	Bullets *m_powerBig = new Bullets(3, "Characters/Bullet/Bullet09_big/BulletAnimation.json");
	m_powerBig->setFilterData(filter);
	m_powerBig->setAnimateForever(true);
	m_powerBullets->addBullets(BULLET_BIG, m_powerBig);

	m_currentBulletType = BULLET_SMALL;
}

int MainCharacter::getTimeOfSkill(std::string name)
{
	return m_skills[name]->getTime();
}

void MainCharacter::setCurrentBulletType(int type)
{
	m_currentBulletType = type;
}

void MainCharacter::setPowerState(bool v)
{
	m_isPowerState = v;
}

bool MainCharacter::isRuningSkill(std::string name)
{
	return m_skills[name]->isRunning();
}

void MainCharacter::startParticleFire()
{
	m_particleFire->start();
}

void MainCharacter::stopParticleFire()
{
	m_particleFire->stop();
}

void MainCharacter::update(float deltaTime)
{
	if (!m_isAlive)
		return;

	DynamicGameObject::update(deltaTime);
	m_bullets->update(deltaTime);
	m_powerBullets->update(deltaTime);

	m_particleFire->setPosition(Vec2(m_sprite->getPosition().x, m_sprite->getPosition().y + getActualSize().height / 2));

	if (m_HP <= 0 || (getPosition()).y < -200)
		die();
}

void MainCharacter::idle()
{
	if (m_isPowerState)
		runAction(MainCharacter::STATE_FLY_ON_STICK);
	else
		runAction(MainCharacter::STATE_IDLE);
}

void MainCharacter::addParent(Layer *parent)
{
	DynamicGameObject::addParent(parent, 1);
	m_bullets->addParent(parent);
	m_powerBullets->addParent(parent);
	m_parent->addChild(m_particleFire, 0);
}

void MainCharacter::createPhysicsBody(b2World *physicsWorld)
{
	DynamicGameObject::createPhysicsBody(physicsWorld);
	m_physicsBody->SetBullet(true);
	m_bullets->createPhysicsBody(physicsWorld);
	m_powerBullets->createPhysicsBody(m_physicsWorld);
}

void MainCharacter::moveRight(float deltaTime)
{
	m_sprite->setFlippedX(false);
	b2Vec2 position = m_physicsBody->GetPosition();
	position.x += b2Helper::asb2Value(m_runStep * RATIO_RUN_STEP * deltaTime);
	m_physicsBody->SetTransform(position, 0);

	if (nContactWithGround > 0) {
		int action = (m_runStep >= 9999999) ? MainCharacter::STATE_FLY_ON_STICK :
			MainCharacter::STATE_WALK;
		runAction(action);
	}
}

void MainCharacter::moveLeft(float deltaTime)
{
	m_sprite->setFlippedX(true);
	b2Vec2 position = m_physicsBody->GetPosition();
	position.x -= b2Helper::asb2Value(m_runStep * RATIO_RUN_STEP * deltaTime);
	m_physicsBody->SetTransform(position, 0);

	if (nContactWithGround > 0) {
		int action = (m_runStep >= 9999999) ? MainCharacter::STATE_FLY_ON_STICK :
			MainCharacter::STATE_WALK;
		runAction(action);
	}
}

void MainCharacter::jumpUp()
{
	if (!isRunningAction(MainCharacter::STATE_JUMP) && nContactWithGround > 0) {
		AudioConfig::getInstance()->playEffect("JUMP");
		m_physicsBody->ApplyLinearImpulse(b2Vec2(0, b2Helper::asb2Value(600)), m_physicsBody->GetPosition(), true);
		runAction(MainCharacter::STATE_JUMP);
	}
}

int MainCharacter::getMagic()
{
	return m_magic;
}

void MainCharacter::nextBullet() 
{
	runActionWithCallBack(MainCharacter::STATE_HIT, CallFunc::create([this]() {
		auto currentPos = getPosition();
		currentPos.y += m_sprite->getContentSize().height / 2;
		cocos2d::Vec2 direct(0, 0);
		int sign = m_sprite->isFlippedX() ? -1 : 1;
		currentPos.x += sign * m_sprite->getContentSize().width + 50;
		direct.x += sign * 600;

		if (m_isPowerState)
			m_powerBullets->nextBullet(m_currentBulletType, currentPos, direct);
		else
			m_bullets->nextBullet(m_currentBulletType, currentPos, direct);
	}));
}

void MainCharacter::die()
{
	m_isAlive = false;
	removePhysicsBody();

	runActionWithCallBack(STATE_DIE, CallFunc::create([this]() {
		this->setVisible(false);
		Helper::waitAndExecute(2000, []() {
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
				GameObjectManager::getInstance()->getGameHub()->setEnable(false);
				GameObjectManager::getInstance()->getGameHub()->showAlert(GameHub::LOSE);
			});
		});
	}));
	AudioConfig::getInstance()->playEffect("GAMEOVER");
}

void MainCharacter::hitWithStick()
{
	if (!isRunningAction(MainCharacter::STATE_HIT_WITH_STICK)) {
		static int id = 0;
		float duration = m_actions[MainCharacter::STATE_HIT_WITH_STICK]->getDuration();
		Helper::waitAndExecute(duration / 2 * 1000, []() {
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
				AudioConfig::getInstance()->playEffect("HITWITHSTICK");
			});
		});
		runAction(MainCharacter::STATE_HIT_WITH_STICK);
		clearAttackedObject(STATE_HIT_WITH_STICK);
	}
}

void MainCharacter::attack(LivingObject *lObj)
{
	if (isRunningAction(STATE_HIT_WITH_STICK)) {
		if (canAttack(STATE_HIT_WITH_STICK, lObj)) {
			lObj->pReceiveDamage(m_hitWithStickDamage);
			addAttackedObject(STATE_HIT_WITH_STICK, lObj);
		}
	}
}

int MainCharacter::getHP()
{
	return m_HP;
}

int MainCharacter::getMaxHP()
{
	return m_maxHP;
}

int MainCharacter::getEnergy()
{
	return m_energy;
}

void MainCharacter::addHitWithStickDamage(int number)
{
	m_hitWithStickDamage += number;
}

int MainCharacter::getMaxEnergy()
{
	return m_maxEnergy;
}

void MainCharacter::addRunStep(int number)
{
	m_runStep += number;
}

void MainCharacter::addHP(int number)
{
	if (this->m_HP < this->m_maxHP)
	{
		if (this->m_HP + number >= this->m_maxHP)
		{
			this->m_HP = this->m_maxHP;
		}
		else
		{
			this->m_HP += number;
		}
	}
}

void MainCharacter::addEnergy(int number)
{
	if (this->m_energy < this->m_maxEnergy)
	{
		if (this->m_energy + number >= this->m_maxEnergy)
		{
			this->m_energy = this->m_maxEnergy;
		}
		else
		{
			this->m_energy += number;
		}
	}
}

void MainCharacter::subEnergy(int number)
{
	if (this->m_energy >= number)
	{
		this->m_energy -= number;
	}
	else
	{
		this->m_energy = 0;
	}
}

void MainCharacter::setRunStep(int number)
{
	this->m_runStep = number;
}

int MainCharacter::getRunStep()
{
	return this->m_runStep;
}

void MainCharacter::gethit()
{
	AudioConfig::getInstance()->playEffect("GETHIT");
	if (isRunningAction(STATE_FLY))
		runAction(STATE_FLY_GETHIT);
	else if (isRunningAction(STATE_FLY_ON_STICK))
		runAction(STATE_FLY_ON_STICK_GETHIT);
	else
		runAction(STATE_GETHIT);
}

void MainCharacter::addContactWithGround(int v)
{
	nContactWithGround += v;
	nContactWithGround = max(nContactWithGround, 0);
}

MainCharacter::~MainCharacter()
{	
	delete m_bullets;
}