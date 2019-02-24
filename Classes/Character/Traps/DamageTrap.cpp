#include "DamageTrap.h"

DamegeTrap::DamegeTrap()
{
	m_sprite = NULL;
}

void DamegeTrap::collision(LivingObject *lObj, void *data)
{
	if (canAttack(lObj, 0)) {
		addAttackedObject(lObj, 0);
		eraseAttackedObjectAfter(lObj, 0, 1000);
		lObj->pReceiveDamage(m_damage);
	}
}

void DamegeTrap::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
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