#include "Helper\Helper.h"
#include "Character\Monsters\Spider.h"
#include "Character\Monsters\BossSmall.h"
//#include "Character\Monsters\Boss04.h"
#include "Character\Monsters\BossSmall.h"
#include "Character\Monsters\runMonster.h"
#include "Character\Monsters\stupidMonster.h"
#include "Character\Monsters\BoxMonster.h"
#include "Character\Monsters\FrogMonster.h"
#include "Character\Monsters\BirdMonster.h"
#include "Character\Traps\FallingTrap.h"
#include "Character\Traps\MovingTrap.h"
#include "Character\Traps\FallingTrap2.h"
#include "Character\Traps\DamageTrap.h"
#include "Character\Traps\GunTrap.h"
#include "Character\Traps\StaticTrap.h"
#include"Character\Traps\\FireTrap.h"
#include "Character\Monsters\runMonster.h"
#include "Character\Monsters\BatMonster.h"
#include"Character\Monsters\Box.h"


Monster* Helper::createMonsterByType(const std::string &type)
{
	Monster *monster;
	
	if (type == "Spider")
		monster = new Spider();
	//else if (type == "Boss04")
	//	monster = new Boss04();
	else if (type == "BossSmall")
		monster = new BossSmall();
	else if (type == "runMonster")
		monster = new RunMonster();
	else if (type == "stupidMonster")
		monster = new StupidMonster();
	else if (type == "BoxMonster")
		monster = new BoxMonster();
	else if (type == "Box")
		monster = new Box();
	else if (type == "FrogMonster")
		monster = new FrogMonster();
	else if (type == "BirdMonster")
		monster = new BirdMonster();


	return monster;
}

Trap* Helper::createTrapByType(const std::string &type)
{
	Trap *trap;

	if (type == "FallingTrap")
		trap = new FallingTrap();
	else if (type == "MovingTrap")
		trap = new MovingTrap();
	else if (type == "FallingTrap2")
		trap = new FallingTrap2();
	else if (type == "DamegeTrap")
		trap = new DamegeTrap();
	else if (type == "GunTrap")
		trap = new GunTrap();
	else if (type == "FireTrap")
		trap = new FireTrap();
	else if (type == "StaticTrap")
		trap = new StaticTrap();
	else if (type == "BatMonster")
		trap = new BatMonster();
	
	return trap;
}

void Helper::waitAndExecute(long long delay_ms, std::function<void(void)> func)
{
	std::thread m_thread([delay_ms, func]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		func();
		//(*func)();
		//delete func;
	});
	m_thread.detach();
}