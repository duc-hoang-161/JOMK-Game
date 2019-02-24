#include "Character\Monsters\BoxMonster.h"
#include "Helper\Helper.h"

const int BoxMonster::STATE_WALK = 1;
const int BoxMonster::STATE_ATTACK = 2;
const int BoxMonster::STATE_IDLE = 3;


BoxMonster::BoxMonster()
	: Monster("Characters/BoxMonster/BoxMonsterAnimation.json")
{

	m_traceDistance = 200;
	m_healthBarBackground->setVisible(false);
	m_healthBar->setVisible(false);

}

void BoxMonster::update(float deltaTime)
{
	Monster::update(deltaTime);	// super
	
	if (m_HP <= 0) {
		CallFunc *func = CallFunc::create([this]() {
			this->setVisible(false);
		});

		runActionWithCallBack(BoxMonster::STATE_IDLE, func);
	}
}

bool BoxMonster::attack(LivingObject *lObj)
{
	Monster::attack(lObj);

	// quy ước 0 là chạm vào monster
	// sau 0.85 sẽ reset lại
	if (canAttack(0, lObj)) {
		runAction(BoxMonster::STATE_ATTACK);
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

BoxMonster::~BoxMonster()
{

}