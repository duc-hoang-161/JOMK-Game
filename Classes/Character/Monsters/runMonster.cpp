#include "Character\Monsters\runMonster.h"
#include "Helper\Helper.h"

const int RunMonster::STATE_WALK = 1;
const int RunMonster::STATE_DIE = 2;

RunMonster::RunMonster()
	: Monster("Characters/runMonster/runMonsterAnimation.json")
{
	m_traceDistance = 200;
}

void RunMonster::update(float deltaTime)
{
	Monster::update(deltaTime);	// super
	
	if (m_HP <= 0) {
	CallFunc *func = CallFunc::create([this]() {
			this->setVisible(false);
		});

		runActionWithCallBack(RunMonster::STATE_DIE, func);
	}
	
	if (m_isAlive) {
		if (originalPosition.x == 0 && originalPosition.y == 0)
			originalPosition = getPosition();

		int max_distance = 250;
		int x = getPosition().x;
		int step = 10;

		if (x < originalPosition.x - max_distance)
		{
			m_sprite->setFlippedX(false);
			isMoveLeft = false;
		}
		else if (x > originalPosition.x + max_distance)
		{
			m_sprite->setFlippedX(true);
			isMoveLeft = true;
		}

		if (isMoveLeft)
			step *= -1;

		//animation
		runAction(RunMonster::STATE_WALK);

		setPosition(Vec2(x + step, getPosition().y));
	}
}

bool RunMonster::attack(LivingObject *lObj)
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
		lObj->pChangeRunStep(500, -0.5);
		return true;
	}

	return false;
}

void RunMonster::moveLeft(float deltaTime)
{
	Monster::moveLeft(deltaTime);
	runAction(STATE_WALK);		
}

void RunMonster::moveRight(float deltaTime)
{
	Monster::moveRight(deltaTime);
	runAction(STATE_WALK);
}

RunMonster::~RunMonster()
{

}