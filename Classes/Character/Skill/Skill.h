#ifndef __SKILL_H__
#define __SKILL_H__

#include <iostream>
#include <functional>
#include "Helper\Helper.h"

class MainCharacter;

class Skill {
protected:
	MainCharacter *m_mainCharacter;
	int m_quantity;
	std::vector<Vec2> m_IDs;
	std::vector<int> m_times;
	std::vector<int> m_manaToUses;

	int m_level;
	long long t_startTime;
	bool m_isRunning;
public:
	Skill(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);

	void setMainCharacter(MainCharacter *mc);
	bool isRunning();
	int getRestTime();
	int getTime();
	virtual void setLevel(int level);
	virtual bool execute();
};

#endif