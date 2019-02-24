#include "IronFist.h"

IronFist::IronFist(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute, SuperBullets *normalState, SuperBullets *superState) :
	Skill(attribute)
{
	auto energys = (*attribute)["Attribute"]["Energy"].GetArray();
	for (auto i = 0; i < energys.Size(); i++)
		m_magicRatios.push_back(energys[i].GetFloat());

	auto rageBuffs = (*attribute)["Attribute"]["RageBuff"].GetArray();
	for (auto i = 0; i < rageBuffs.Size(); i++)
		m_rageBuffs.push_back(rageBuffs[i].GetFloat());

	auto distances = (*attribute)["Attribute"]["Distance"].GetArray();
	for (auto i = 0; i < distances.Size(); i++)
		m_distances.push_back(distances[i].GetInt());

	normalState->getBullets(MainCharacter::BULLET_SMALL)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level]);
		bullet->destroy();
	});
	normalState->getBullets(MainCharacter::BULLET_SMALL)->setMaxDistance(m_distances[m_level]);

	normalState->getBullets(MainCharacter::BULLET_MEDIUM)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level] * 1.25);
		bullet->destroy();
	});
	normalState->getBullets(MainCharacter::BULLET_MEDIUM)->setMaxDistance(m_distances[m_level] + 100);

	normalState->getBullets(MainCharacter::BULLET_BIG)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level] * 1.5);
		bullet->destroy();
	});
	normalState->getBullets(MainCharacter::BULLET_BIG)->setMaxDistance(m_distances[m_level] + 200);

	superState->getBullets(MainCharacter::BULLET_SMALL)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level] * m_rageBuffs[m_level]);
		bullet->destroy();
	});
	superState->getBullets(MainCharacter::BULLET_SMALL)->setMaxDistance(m_distances[m_level]);

	superState->getBullets(MainCharacter::BULLET_MEDIUM)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level] * m_rageBuffs[m_level] * 1.25);
		bullet->destroy();
	});
	superState->getBullets(MainCharacter::BULLET_MEDIUM)->setMaxDistance(m_distances[m_level] + 100);

	superState->getBullets(MainCharacter::BULLET_BIG)->addCollisionCondition
		(BITMASK_MONSTER, [&](Bullet *bullet, LivingObject *object) {
		LivingObject *monster = static_cast<LivingObject*>(object);
		if (monster == NULL) return;
		monster->pReceiveDamage(m_mainCharacter->getMagic() * m_magicRatios[m_level] * m_rageBuffs[m_level] * 1.5);
		bullet->destroy();
	});
	superState->getBullets(MainCharacter::BULLET_BIG)->setMaxDistance(m_distances[m_level] + 200);

	normal = normalState;
	super = superState;
}

void IronFist::setLevel(int level)
{
	Skill::setLevel(level);
	normal->getBullets(MainCharacter::BULLET_SMALL)->setMaxDistance(m_distances[m_level]);
	normal->getBullets(MainCharacter::BULLET_MEDIUM)->setMaxDistance(m_distances[m_level] + 100);
	normal->getBullets(MainCharacter::BULLET_BIG)->setMaxDistance(m_distances[m_level] + 200);

	super->getBullets(MainCharacter::BULLET_SMALL)->setMaxDistance(m_distances[m_level]);
	super->getBullets(MainCharacter::BULLET_MEDIUM)->setMaxDistance(m_distances[m_level] + 100);
	super->getBullets(MainCharacter::BULLET_BIG)->setMaxDistance(m_distances[m_level] + 200);
}

bool IronFist::execute()
{
	if (m_mainCharacter->isRunningAction(MainCharacter::STATE_HIT))
		return false;

	if (!Skill::execute())
		return false;

	m_mainCharacter->nextBullet();

	return true;
}