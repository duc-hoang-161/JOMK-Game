#ifndef __TRAP_H__
#define __TRAP_H__

#include "Objects\GameObject.h"
#include "Objects\LivingObject.h"
#include <set>
#include "Character\MainCharacter\MainCharacter.h"

class MainCharacter;

class Trap : public GameObject {
private:
	std::map<int, std::set<LivingObject*>> m_attackedObject;
protected:
	MainCharacter *m_mainCharacter;
	
	bool canAttack(LivingObject *lObj, int tag);
	void addAttackedObject(LivingObject *lObj, int tag);
	void eraseAttackedObjectAfter(LivingObject *lObj, int tag, int ms);
public:
	Trap(std::string linkImg);
	Trap();
	void addMainCharacter(MainCharacter *mainCharacter);

	virtual void update(float deltaTime);
	virtual void createPhysicsBody(b2World *physicsWorld);
	virtual void collision(LivingObject *lObj, void *data = NULL) = 0;

	std::string className()
	{
		return "Trap";
	}

	virtual void setAttribute(string jsonFile);
	virtual void setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
		< rapidjson::CrtAllocator >> *attribute);
};

#endif