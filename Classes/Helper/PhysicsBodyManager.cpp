#include "Helper\PhysicsBodyManager.h"

PhysicsBodyManager::PhysicsBodyManager(std::string directoryOfJsonFile)
{
	MyBodyParser::getInstance()->parseJsonFile(directoryOfJsonFile + "physicsbody.json", directoryOfJsonFile + "config.json");
}

b2Body* PhysicsBodyManager::get(SpriteFrame* spriteFrame, bool isFlippedX)
{
	if (m_data.find(spriteFrame) != m_data.end()) {
		FixtureDef *fixtureDef = NULL;
		if (isFlippedX)
			fixtureDef = m_data[spriteFrame].first;
		else
			fixtureDef = m_data[spriteFrame].second;

		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.angle = 0;

		b2Body *body = m_physicsWorld->CreateBody(&myBodyDef);

		while (fixtureDef != NULL) {
			b2Fixture *fixture = body->CreateFixture(&fixtureDef->fixture);
			int tag = reinterpret_cast<int>(fixture->GetUserData());
			fixture->SetFilterData(m_filters[tag]);
			fixtureDef = fixtureDef->next;
		}

		return body;
	}
	return NULL;
}

b2Body* PhysicsBodyManager::getDefault(bool isFlippedX)
{
	if (m_data.find(m_defaultFrame) == m_data.end())
		return NULL;

	FixtureDef *fixtureDef = NULL;
	if (isFlippedX)
		fixtureDef = m_data.begin()->second.first;
	else
		fixtureDef = m_data.begin()->second.second;

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.angle = 0;

	b2Body *body = m_physicsWorld->CreateBody(&myBodyDef);

	while (fixtureDef != NULL) {
		b2Fixture *fixture = body->CreateFixture(&fixtureDef->fixture);
		int tag = reinterpret_cast<int>(fixture->GetUserData());
		fixture->SetFilterData(m_filters[tag]);
		fixtureDef = fixtureDef->next;
	}

	return body;
}

void PhysicsBodyManager::add(SpriteFrame* spriteFrame, std::string key)
{
	if (MyBodyParser::getInstance()->containKey(key)) {
		auto pBody = MyBodyParser::getInstance()->bodyFormJson(key);
		m_data[spriteFrame] = pBody;
	}
}

void PhysicsBodyManager::addFilter(int key, b2Filter value)
{
	m_filters[key] = value;
}

void PhysicsBodyManager::setPhysicsWorld(b2World *physicsWorld)
{
	m_physicsWorld = physicsWorld;
}

void PhysicsBodyManager::setDefaultFrame(SpriteFrame *sf)
{
	m_defaultFrame = sf;
}

PhysicsBodyManager::~PhysicsBodyManager()
{
}