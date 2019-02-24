#include "Character\Monsters\StupidMonster.h"
#include "Helper\Helper.h"

const int StupidMonster::STATE_IDLE = 1;
const int StupidMonster::STATE_ATTACK = 2;
const int StupidMonster::STATE_DIE = 3;

StupidMonster::StupidMonster()
	: Monster("Characters/stupidMonster/stupidMonsterAnimation.json")
{
	m_bullets = new Bullets(2, "Characters/Bullet/Bullet07/BulletAnimation.json");
	m_bullets->addCollisionCondition(BITMASK_CHARACTER, [](Bullet* bullet, LivingObject* object) {
		LivingObject *mc = static_cast<LivingObject*>(object);
		if (mc == NULL) return;
		mc->pReceiveDamage(100);
		bullet->destroy();
	});
	m_bullets->setMaxDistance(300);

	m_canShot = true;
}

void StupidMonster::update(float deltaTime)
{
	Monster::update(deltaTime);	// super
	
	if (m_HP <= 0) {
		CallFunc *func = CallFunc::create([this]() { 
			this->setVisible(false);
		});

		runActionWithCallBack(StupidMonster::STATE_DIE, func);
	}
	m_bullets->update(deltaTime);

	if (m_isAlive) {
		Vec2 pos = m_mainCharacter->getPosition();
		Vec2 pos1 = getPosition();
		int step = 0;
		if (abs(pos.x - pos1.x) > 300 || abs(pos.x - pos1.x) < 100 && abs(pos.y - pos1.y) > 100){
			if (m_sprite->getNumberOfRunningActions() == 0)
				runAction(StupidMonster::STATE_IDLE);
		}

		if (abs(pos.x - pos1.x) < 300 && abs(pos.x - pos1.x)>100 && pos.x < pos1.x)
		{
			m_sprite->setFlippedX(true);
			runAction(StupidMonster::STATE_ATTACK);
			step -= 5;

		}
		if (abs(pos.x - pos1.x) < 300 && abs(pos.x - pos1.x)>100 && pos.x > pos1.x)
		{
			m_sprite->setFlippedX(false);
			runAction(StupidMonster::STATE_ATTACK);
			step += 5;
		}
		if (abs(pos.x - pos1.x) < 100 && abs(pos.y - pos1.y) < 100)
		{
			runAction(StupidMonster::STATE_ATTACK);
		}

		setPosition(Vec2(pos1.x + step, pos1.y));

		if (m_mainCharacter->getPosition().distance(getPosition()) < 500) {
			if (m_mainCharacter->getPosition().x < getPosition().x)
				m_sprite->setFlippedX(true);
			else
				m_sprite->setFlippedX(false);

			shot();
			
		}
	}
}

void StupidMonster::addParent(Layer *layer)
{
	Monster::addParent(layer);
	m_bullets->addParent(layer);
}

void StupidMonster::createPhysicsBody(b2World *physicsWorld)
{
	Monster::createPhysicsBody(physicsWorld);
	m_bullets->createPhysicsBody(physicsWorld);
}

bool StupidMonster::attack(LivingObject *lObj)
{
	Monster::attack(lObj);

	// quy ước 0 là chạm vào monster
	// sau 0.85 sẽ reset lại
	if (canAttack(0, lObj)) {
		addAttackedObject(0, lObj);
		
		std::function<void(void)> func([this, lObj]() {
			this->removeAttackedObject(0, lObj);
		});
		Helper::waitAndExecute(850, func);

		lObj->pReceiveDamage(m_damage);
		return true;
	}

	return false;
}

void StupidMonster::moveLeft(float deltaTime)
{
	Monster::moveLeft(deltaTime);
	runAction(STATE_ATTACK);		
}

void StupidMonster::shot()
{
	if (m_canShot) {
		runActionWithCallBack(StupidMonster::STATE_ATTACK, CallFunc::create([&]() {
			auto currentPos = getPosition();
			cocos2d::Vec2 direct;
			direct.set(0, 0);

			if (m_sprite->isFlippedX()) {
				currentPos.x -= m_sprite->getContentSize().width + 50;
				direct.x = -600;
			}
			else {
				currentPos.x += m_sprite->getContentSize().width + 50;
				direct.x = 600;
			}

			m_bullets->nextBullet(currentPos, direct);
		}));

		m_canShot = false;
		Helper::waitAndExecute(2500, [&]() {
			m_canShot = true;
		});
	}	
}

void StupidMonster::moveRight(float deltaTime)
{
	Monster::moveRight(deltaTime);
	runAction(STATE_ATTACK);
}

StupidMonster::~StupidMonster()
{

}