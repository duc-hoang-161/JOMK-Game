#include "Door.h"
#include "GameManager\GameObjectManager.h"
#include "GameManager\GameObjectManager.h"

const int Door::OPEN = 1;

Door::Door(string animationFileDirect) 
	: DynamicGameObject(animationFileDirect)
{
	b2Filter filter;
	filter.categoryBits = BITMASK_DOOR;
	filter.maskBits = BITMASK_GROUND | BITMASK_CHARACTER;
	setFilterData(filter);

	m_isOpened = false;
	m_isResetPB = false;
}

void Door::open()
{
	if (GameObjectManager::getInstance()->getGameHub()->isHaveKey() && !m_isOpened)
		runAction(OPEN),
		m_isOpened = true;
	
	if (m_isOpened && m_isResetPB)
		GameObjectManager::getInstance()->nextLevel();
}

void Door::update(float deltaTime)
{
	if (m_isOpened && !m_isResetPB) {
		m_isResetPB = true;
		
		m_physicsBody->SetTransform(b2Helper::asb2Vec2(getPosition()), 0);
		m_physicsBody->SetType(b2_staticBody);

		b2PolygonShape shape;
		shape.SetAsBox(0.125, 0.125);

		// Khai báo fixtureDef;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &shape;
		myFixtureDef.friction = 1;
		myFixtureDef.filter = m_filterData;

		b2Fixture *fix = m_physicsBody->GetFixtureList();
		m_physicsBody->DestroyFixture(fix);
		m_physicsBody->CreateFixture(&myFixtureDef); //thêm một fixture vào body
	}
}