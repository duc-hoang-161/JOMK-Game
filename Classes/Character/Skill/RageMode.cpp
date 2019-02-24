#include "RageMode.h"

RageMode::RageMode(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute) : Skill(attribute)
{
	auto effects = (*attribute)["Attribute"]["Effect"].GetArray();
	for (auto i = 0; i < effects.Size(); i++)
		m_effects.push_back(effects[i].GetInt());

	auto buffs = (*attribute)["Attribute"]["Buff"].GetArray();
	for (auto i = 0; i < buffs.Size(); i++)
		m_buffs.push_back(buffs[i].GetFloat());
}

bool RageMode::execute()
{
	if (!Skill::execute())
		return false;

	static int buffDamage = m_mainCharacter->getHitWithStickDamage() * (m_buffs[m_level] - 1);
	static int buffStep = m_mainCharacter->getRunStep() * 0.25;
	m_mainCharacter->addHitWithStickDamage(buffDamage);
	m_mainCharacter->addRunStep(buffStep);

	m_mainCharacter->setPowerState(true);
	m_mainCharacter->startParticleFire();

	Helper::waitAndExecute(m_effects[m_level] * 1000, [&]() {
		m_mainCharacter->stopParticleFire();
		m_mainCharacter->setPowerState(false);
		m_mainCharacter->addHitWithStickDamage(-buffDamage);
		m_mainCharacter->addRunStep(-buffStep);
	});

	return true;
}