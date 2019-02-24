#include "LivingObject.h"
#include "Helper\Helper.h"

LivingObject::LivingObject()
{
	m_pPhysicsBody = NULL;
	m_pHP = NULL;
	m_pMaxHP = NULL;
	m_pArmor = NULL;
	m_pRunStep = NULL;
	m_pSprite = NULL;
}

void LivingObject::pRegisterPhysicsBody(b2Body **physicsBody)
{
	m_pPhysicsBody = physicsBody;
}

void LivingObject::pRegisterSprite(cocos2d::Sprite **sprite)
{
	m_pSprite = sprite;
}

void LivingObject::pRegisterHP(int *HP)
{
	m_pHP = HP;
}

void LivingObject::pRegisterMaxHP(int *maxHP)
{
	m_pMaxHP = maxHP;
}

void LivingObject::pRegisterArmor(int *armor)
{
	m_pArmor = armor;
}

void LivingObject::pRegisterRunStep(int *runStep)
{
	m_pRunStep = runStep;
}

b2Vec2 LivingObject::pGetPosition()
{
	return (*m_pPhysicsBody)->GetPosition();
}

void LivingObject::addAttackedObject(int damageType, LivingObject *lObj)
{
	m_attackedObject[damageType].insert(lObj);
}

void LivingObject::removeAttackedObject(int damageType, LivingObject *lObj)
{
	m_attackedObject[damageType].erase(lObj);
}

void LivingObject::clearAttackedObject(int damageType)
{
	m_attackedObject[damageType].clear();
}

bool LivingObject::canAttack(int damageType, LivingObject *lObj)
{
	return m_attackedObject[damageType].find(lObj) == m_attackedObject[damageType].end();
}

int LivingObject::pGetRunStep()
{
	if (*m_pRunStep != NULL)
		return *m_pRunStep;
	return 0;
}

void LivingObject::pChangePosition(b2Vec2 deltaPosition)
{
	b2Body *body = *m_pPhysicsBody;
	auto pos = body->GetPosition();
	pos.x += deltaPosition.x;
	pos.y += deltaPosition.y;
	body->SetTransform(pos, 0);
}

void LivingObject::pReceiveDamage(int damage)
{
	if (m_pHP != NULL) {
		int armor = (m_pArmor != NULL) ? *m_pArmor : 0;
		damage = std::max(damage - armor, 0);
		*m_pHP = std::max(0, *m_pHP - damage);

		if (m_pSprite != NULL) {
			auto scene = (*m_pSprite)->getParent();

			std::string c = (damage == 0) ? "" : cocos2d::StringUtils::format("-%d", damage);
			auto myLabel = cocos2d::Label::createWithTTF(c, "fonts/Marker Felt.ttf", 30);
			myLabel->setColor(cocos2d::Color3B(255, 0, 0));

			cocos2d::Vec2 delta;
			auto size = (*m_pSprite)->getContentSize();
			auto pos = (*m_pSprite)->getPosition();
			delta.x = (0.5 - (*m_pSprite)->getAnchorPoint().x) * size.width;
			delta.y = (1.0 - (*m_pSprite)->getAnchorPoint().y) * size.height + 20;
			pos.x += delta.x, pos.y += delta.y;

			myLabel->setPosition(pos);
			scene->addChild(myLabel);

			cocos2d::Sequence *s = cocos2d::Sequence::create(
				cocos2d::Spawn::create(
				cocos2d::MoveBy::create(1, cocos2d::Vec3(0,30,0)),
				cocos2d::FadeOut::create(1),
				nullptr
				),
				cocos2d::CallFunc::create([scene, myLabel]() {
					scene->removeChild(myLabel);
				}), 
				nullptr);

			myLabel->runAction(s);
		}
	}
}

void LivingObject::pChangeRunStep(int time, float percent)
{
	int old = (*m_pRunStep != NULL) ? *m_pRunStep : 0;

	int newRunStep = old + percent * old;

	if (m_pRunStep != NULL) {
		*m_pRunStep = newRunStep;
		Helper::waitAndExecute(time, [&, old]() {
			*m_pRunStep = old;
		});
	}
}