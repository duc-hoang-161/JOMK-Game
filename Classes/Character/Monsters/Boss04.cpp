//#include "Character\Monsters\Boss04.h"
//#include "Helper\Helper.h"
//
//const int Boss04::STATE_IDLE = 1;
//const int Boss04::STATE_WALK = 2;
//const int Boss04::STATE_ATTACK = 3;
//const int Boss04::STATE_DIE = 4;
//const int Boss04::STATE_JUMP = 5;
//const int Boss04::STATE_RUN = 6;
//const int Boss04::STATE_HURT = 7;
//
//Boss04::Boss04()
//	: Monster("Characters/Boss04/Boss04Animation.json")
//{
//	m_sprite->setAnchorPoint(Vec2(0.5, 0));
//
//	originalPosition.x = originalPosition.y = -1;
//	m_traceDistance = 200;
//
//	m_bullets = new Bullets(5, "Characters/Bullet/Bullet01/BulletAnimation.json");
//	m_bullets->addCollisionCondition(BITMASK_CHARACTER, [](Bullet* bullet, LivingObject* object) {
//		LivingObject *mc = static_cast<LivingObject*>(object);
//		if (mc == NULL) return;
//		mc->pReceiveDamage(50);
//		bullet->destroy();
//	});
//	m_bullets->setMaxDistance(300);
//}
//
//void Boss04::update(float deltaTime)
//{
//	Monster::update(deltaTime);	// super
//
//	if (m_HP <= 0) {
//		CallFunc *func = CallFunc::create([this]() {
//			this->setVisible(false);
//		});
//
//		runActionWithCallBack(Boss04::STATE_DIE, func);
//	}
//
//	if (m_isAlive) {
//		if (originalPosition.x == -1 && originalPosition.y == -1)
//			originalPosition = getPosition();
//
//		int max_distance = 150;
//		int x = getPosition().x;
//
//		if (x < originalPosition.x - max_distance)
//			isMoveLeft = false;
//		else if (x > originalPosition.x + max_distance)
//			isMoveLeft = true;
//
//		isMoveLeft ? moveLeft() : moveRight();
//
//		if (m_sprite->getNumberOfRunningActions() == 0)
//			runAction(Boss04::STATE_IDLE);
//
//
//		if (m_mainCharacter->getPosition().distance(getPosition()) < 500) {
//			if (m_mainCharacter->getPosition().x < getPosition().x)
//				m_sprite->setFlippedX(true);
//			else
//				m_sprite->setFlippedX(false);
//
//			shot();
//		}
//	}
//}
//
//void Boss04::moveLeft()
//{
//	Monster::moveLeft();
//	runAction(STATE_WALK);	
//}
//
//void Boss04::moveRight()
//{
//	Monster::moveRight();
//	runAction(STATE_WALK);
//}
//
//void Boss04::shot()
//{
//	auto currentPos = getPosition();
//	currentPos.y += m_sprite->getContentSize().height / 2;
//	cocos2d::Vec2 direct;
//	direct.set(0, 0);
//
//	if (m_sprite->isFlippedX()) {
//		currentPos.x -= m_sprite->getContentSize().width + 50;
//		direct.x = -600;
//	}
//	else {
//		currentPos.x += m_sprite->getContentSize().width + 50;
//		direct.x = 600;
//	}
//
//	m_bullets->nextBullet(currentPos, direct);
//}
//
//bool Boss04::attack(LivingObject *lObj)
//{
//	Monster::attack(lObj);
//
//	 quy ước 0 là chạm vào monster
//	 sau 0.85 sẽ reset lại
//	if (canAttack(0, lObj)) {
//		this->addAttackedObject(0, lObj);
//
//		std::function<void(void)> func([this, lObj]() {
//			this->removeAttackedObject(0, lObj);
//		});
//		Helper::waitAndExecute(850, func);
//
//		lObj->pReceiveDamage(m_damage);
//		return true;
//	}
//
//	return false;
//}
//
//Boss04::~Boss04()
//{
//
//}