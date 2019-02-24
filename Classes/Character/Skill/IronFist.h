#ifndef __IRON_FISH_H__
#define __IRON_FISH_H__

#include "Character\Skill\Skill.h"

class IronFist : public Skill {
private:
	vector<int> m_distances;
	vector<float> m_magicRatios;
	vector<float> m_rageBuffs;

	SuperBullets *normal, *super;
public:
	IronFist(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute, SuperBullets *normalState, SuperBullets *superState);

	void setLevel(int level);
	bool execute();
};

#endif