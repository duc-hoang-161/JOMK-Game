#include "FireTrap.h"

FireTrap::FireTrap()
{
	m_sprite = NULL;
	m_bullets = new Bullets(5, "Characters/Bullet/Bullet06/BulletAnimation.json");
	m_bullets->addCollisionCondition(BITMASK_CHARACTER, [](Bullet* bullet, LivingObject* object) {
		LivingObject *mc = static_cast<LivingObject*>(object);
		if (mc == NULL) return;
		mc->pReceiveDamage(100);
	});
	m_bullets->setMaxDistance(100);
	m_bullets->setTime(3000);

	m_canShot = true;
}

void FireTrap::collision(LivingObject *lObj, void *data)
{
	if (canAttack(lObj, 0)) {
		addAttackedObject(lObj, 0);
		eraseAttackedObjectAfter(lObj, 0, 1000);
		lObj->pReceiveDamage(m_damage);
	}
}

void FireTrap::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("Sprite") != attribute->MemberEnd()) {
		auto sprite = &(*attribute)["Sprite"];

		if (sprite->FindMember("Image") != sprite->MemberEnd()) {
			std::string link = (*sprite)["Image"].GetString();
			m_sprite = Sprite::create(link);
			m_sprite->retain();
		}

		if (sprite->FindMember("Scale") != sprite->MemberEnd())
			m_sprite->setScale((*sprite)["Scale"].GetFloat());
	}

	if (attribute->FindMember("MinDistance") != attribute->MemberEnd())
		m_distanceToFall = (*attribute)["MinDistance"].GetInt();
	if (attribute->FindMember("Damage") != attribute->MemberEnd())
		m_damage = (*attribute)["Damage"].GetInt();
}
void FireTrap::shot()
{
	if (m_canShot) {

		auto currentPos = getPosition();
		cocos2d::Vec2 direct;
		direct.set(0, 0);
		currentPos.y += m_sprite->getContentSize().height;
		direct.y = 200;

		m_bullets->nextBullet(currentPos, direct);

		m_canShot = false;
		Helper::waitAndExecute(4000, [&]() {
			m_canShot = true;
		});
	}

}
void FireTrap::addParent(Layer *layer)
{
	Trap::addParent(layer);
	m_bullets->addParent(layer);
}
void FireTrap::createPhysicsBody(b2World *physicsWorld)
{
	Trap::createPhysicsBody(physicsWorld);
	m_bullets->createPhysicsBody(physicsWorld);
}
void FireTrap::update(float datatime)
{
	Trap::update(datatime);
	m_bullets->update(datatime);
	if (m_mainCharacter->getPosition().distance(getPosition()) < 500) {
		shot();

	}
}