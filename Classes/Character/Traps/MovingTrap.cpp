#include "MovingTrap.h"

MovingTrap::MovingTrap()
{
	m_sprite = NULL;
	m_orientation = 0;
}

void MovingTrap::collision(LivingObject *lObj, void *data)
{

}

void MovingTrap::update(float deltaTime)
{
	if (m_sprite != NULL) {
		Vec2 pos = m_sprite->getPosition();
		m_physicsBody->SetTransform(b2Helper::asb2Vec2(pos), 0);
	}
}

void MovingTrap::createPhysicsBody(b2World *physicsWorld)
{
	Trap::createPhysicsBody(physicsWorld);
	b2FixtureDef myFixtureDef;
	myFixtureDef.restitution = 0;
}

void MovingTrap::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("Sprite") != attribute->MemberEnd()) {
		auto sprite = &(*attribute)["Sprite"];

		if (sprite->FindMember("Image") != sprite->MemberEnd()) {
			string link = (*sprite)["Image"].GetString();
			m_sprite = Sprite::create(link);
			m_sprite->retain();

			if (sprite->FindMember("Scale") != sprite->MemberEnd())
				m_sprite->setScale((*sprite)["Scale"].GetFloat());
		}
	}

	if (attribute->FindMember("Distance") != attribute->MemberEnd()) {
		m_distance = (*attribute)["Distance"].GetInt();

		if (attribute->FindMember("Orientation") != attribute->MemberEnd()) {
			string tmp = (*attribute)["Orientation"].GetString();
			if (tmp == "Vertical")
				m_orientation = 0;
			else if (tmp == "Horizontal")
				m_orientation = 1;
			else if (tmp == "Verticalfast")
				m_orientation = 2;
		}

		moveForever();
	}	
}

void MovingTrap::moveForever()
{
	if (m_sprite != NULL) {
		if (m_orientation == 0) {
			auto move = Sequence::createWithTwoActions(MoveBy::create(1.5f, Vec2(0, m_distance)), MoveBy::create(3, Vec2(0, -m_distance)));
			m_sprite->runAction(RepeatForever::create(move));
		}
		else if (m_orientation == 1) {
			auto move = Sequence::createWithTwoActions(MoveBy::create(2, Vec2(m_distance, 0)), MoveBy::create(2, Vec2(-m_distance, 0)));
			m_sprite->runAction(RepeatForever::create(move));
		}
		if (m_orientation == 2) {
			auto move = Sequence::createWithTwoActions(MoveBy::create(6, Vec2(0, m_distance)), MoveBy::create(1.5, Vec2(0, -m_distance)));
			m_sprite->runAction(RepeatForever::create(move));
		}
	}
}