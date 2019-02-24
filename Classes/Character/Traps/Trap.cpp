#include"Character/Traps/Trap.h"
#include"Objects/GameObject.h"
#include "Helper\Helper.h"
Trap::Trap(string linkImg)
{
	m_sprite = Sprite::create(linkImg);
	m_sprite->retain();

	m_filterData.categoryBits = BITMASK_TRAP;
	m_filterData.maskBits = -1;
}

Trap::Trap()
{
	m_filterData.categoryBits = BITMASK_TRAP;
	m_filterData.maskBits = -1;
}

void Trap::addMainCharacter(MainCharacter *mainCharacter)
{
	m_mainCharacter = mainCharacter;
}

void Trap::update(float deltaTime)
{
	GameObject::update(deltaTime);
}

bool Trap::canAttack(LivingObject *lObj, int tag)
{
	return m_attackedObject[tag].find(lObj) == m_attackedObject[tag].end();
}

void Trap::addAttackedObject(LivingObject *lObj, int tag)
{
	m_attackedObject[tag].insert(lObj);
}

void Trap::eraseAttackedObjectAfter(LivingObject *lObj, int tag, int ms)
{
	std::set<LivingObject*> *l = &m_attackedObject[tag];
	std::function<void(void)> func([l, lObj](){
		l->erase(lObj);
	});

	Helper::waitAndExecute(ms, func);
}

void Trap::createPhysicsBody(b2World *physicsWorld)
{
	GameObject::createPhysicsBody(physicsWorld);
	m_physicsBody->SetType(b2_staticBody);
}

void Trap::setAttribute(string jsonFile)
{
	string content = FileUtils::getInstance()
		->getStringFromFile(jsonFile);
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	setAttribute(&document);
}

void Trap::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{

}