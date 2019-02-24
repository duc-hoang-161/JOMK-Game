#ifndef __RAGE_MODE_H__
#define __RAGE_MODE_H__

#include "Character\Skill\Skill.h"

class RageMode : public Skill {
private:
	vector<int> m_effects;
	vector<float> m_buffs;
public:
	RageMode(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	bool execute();
};

#endif