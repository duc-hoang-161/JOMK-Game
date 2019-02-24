#ifndef __HELPER_H__
#define __HELPER_H__

#include <iostream>
#include <string>
#include <thread>

#include "Character\Monsters\Monster.h"
#include "Character\Traps\Trap.h"

class Monster;
class Trap;

class Helper {
public:
	static Monster* createMonsterByType(const std::string &type);
	static Trap* createTrapByType(const std::string &type);

	static void waitAndExecute(long long delay_ms, std::function<void(void)> func);
};

#endif