#include "Character\Traps\StaticTrap.h"

StaticTrap::StaticTrap()
{
	m_sprite = NULL;
	
}
 
void StaticTrap::collision(LivingObject *lObj, void *data)
{
	if (canAttack(lObj, 0)) {
		addAttackedObject(lObj, 0);
		eraseAttackedObjectAfter(lObj, 0, 1000);
		lObj->pReceiveDamage(m_damage);
	}
}

void StaticTrap::update(float deltaTime)
{
	if (m_sprite != NULL) {
		Vec2 pos = m_sprite->getPosition();
		m_physicsBody->SetTransform(b2Helper::asb2Vec2(pos), 0);
	}
}

void StaticTrap::createPhysicsBody(b2World *physicsWorld)
{
	Trap::createPhysicsBody(physicsWorld);
}

void StaticTrap::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("Sprite") != attribute->MemberEnd()) {
		auto sprite = &(*attribute)["Sprite"];

		if (sprite->FindMember("Image") != sprite->MemberEnd()) {
			string link = (*sprite)["Image"].GetString();
			m_sprite = Sprite::create(link);
			m_sprite->retain();

			if (sprite->FindMember("Scale") != sprite->MemberEnd())
				m_sprite->setScale((*sprite)["Scale"].GetFloat());
			if (attribute->FindMember("Damage") != attribute->MemberEnd())
				m_damage = (*attribute)["Damage"].GetInt();
		}
	}
}
