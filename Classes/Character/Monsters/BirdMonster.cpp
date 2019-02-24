#include "Character\Monsters\BirdMonster.h"
#include "Helper\Helper.h"

const int BirdMonster::STATE_DIE = 3;
const int BirdMonster::STATE_ATTACK = 4;
const int BirdMonster::STATE_IDLE = 1;
const int BirdMonster::STATE_WALK = 2;


BirdMonster::BirdMonster()
	: Monster("Characters/BirdMonster/BirdMonsterAnimation.json")
{
	m_traceDistance = 200;
}

void BirdMonster::update(float deltaTime)
{
	Monster::update(deltaTime);	// super

	if (m_HP <= 0) {

		CallFunc *func = CallFunc::create([this]() {
			this->setVisible(false);

		});

		runActionWithCallBack(BirdMonster::STATE_DIE, func);
	}
	if (m_isAlive) {
		Vec2 pos = m_mainCharacter->getPosition();
		Vec2 pos1 = getPosition();
		int step = 0;
		if (abs(pos.x - pos1.x) > 300 || abs(pos.x - pos1.x) < 100 && abs(pos.y - pos1.y) > 100){
			runAction(BirdMonster::STATE_IDLE);
		}

		if (abs(pos.x - pos1.x) < 500 && pos.x < pos1.x&&abs(pos.y - pos1.y) < 50)
		{

			m_sprite->setFlippedX(true);
			runAction(BirdMonster::STATE_WALK);
			step -= 2;

		}
		if (abs(pos.x - pos1.x) < 500 && pos.x > pos1.x&&abs(pos.y - pos1.y) < 50)
		{

			m_sprite->setFlippedX(false);
			runAction(BirdMonster::STATE_WALK);
			step += 2;
		}


		setPosition(Vec2(pos1.x + step, pos1.y));
	}

}

bool BirdMonster::attack(LivingObject *lObj)
{
	Monster::attack(lObj);

	// quy ước 0 là chạm vào monster
	// sau 0.85 sẽ reset lại
	if (canAttack(0, lObj)) {
		addAttackedObject(0, lObj);
		AudioConfig::getInstance()->playEffect("SPIDERATTACK");
		std::function<void(void)> func([this, lObj]() {
			this->removeAttackedObject(0, lObj);
		});
		Helper::waitAndExecute(850, func);

		lObj->pReceiveDamage(m_damage);
		return true;
	}

	return false;
}

void BirdMonster::moveLeft(float deltaTime)
{
	Monster::moveLeft(deltaTime);
	runAction(STATE_WALK);
}

void BirdMonster::moveRight(float deltaTime)
{
	Monster::moveRight(deltaTime);
	runAction(STATE_WALK);
}

BirdMonster::~BirdMonster()
{

}