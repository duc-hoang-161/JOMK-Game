#ifndef __FLASH_H__
#define __FLASH_H__

#include "Character\Skill\Skill.h"

class Flash : public Skill {
private:
	vector<int> m_buffSpeedTimes;
public:
	Flash(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	bool execute();
};

#endif