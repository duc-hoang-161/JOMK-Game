#include "Flash.h"

Flash::Flash(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute) : Skill(attribute)
{
	auto times = (*attribute)["Attribute"]["BuffSpeedTime"].GetArray();

	for (auto i = 0; i < times.Size(); i++)
		m_buffSpeedTimes.push_back(times[i].GetInt());
}

bool Flash::execute()
{
	if (!Skill::execute())
		return false;

	m_mainCharacter->addRunStep(5);

	Helper::waitAndExecute(m_buffSpeedTimes[m_level] * 1000, [&]() {
		m_mainCharacter->addRunStep(-5);
	});

	return true;
}