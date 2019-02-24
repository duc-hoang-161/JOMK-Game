#ifndef __HEAL_H__
#define __HEAL_H__

#include "Character\Skill\Skill.h"

class Heal : public Skill {
private:
	vector<int> m_buffHPs;
public:
	Heal(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	bool execute();
};

#endif