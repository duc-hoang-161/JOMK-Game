#include "Item.h"
#include "Import\PackConfig.h"

Item::Item()
{
	m_filterData.categoryBits = BITMASK_ITEM;
	m_filterData.maskBits = BITMASK_CHARACTER | BITMASK_GROUND | BITMASK_ITEM;
}

Item::Item(int type, int id)
{
	m_type = type;
	m_id = id;

	GameItem *gi = ItemsConfig::getInstance()->getItem(m_type, m_id);
	m_sprite = Sprite::create(gi->getSrcInGame());
	m_sprite->retain();

	m_filterData.categoryBits = BITMASK_ITEM;
	m_filterData.maskBits = BITMASK_CHARACTER | BITMASK_GROUND | BITMASK_ITEM;

	m_sprite->setVisible(false);
	m_isAlive = true;
}

void Item::createPhysicsBody(b2World *physicsWorld)
{
	m_physicsWorld = physicsWorld;
	m_physicsBody = NULL;
}

void Item::drop(cocos2d::Vec2 position)
{
	m_sprite->setVisible(true);
	setPosition(position);
	GameObject::createPhysicsBody(m_physicsWorld);
	m_physicsBody->SetUserData((void*)this);
	m_isPicked = false;
}

void Item::picked()
{
	m_sprite->setVisible(false);
	m_isPicked = true;
}

void Item::addToPack()
{
	PackConfig::getInstance()->addItem(m_type, m_id);
}

#include "GameManager\GameObjectManager.h"
void Item::update(float deltaTime)
{
	if (m_physicsBody != NULL) {		
		GameObject::update(deltaTime);
		if (m_isPicked && m_isAlive) {	// Nếu đã được nhặt
			removePhysicsBody();
			m_physicsBody = NULL;
			m_isAlive = false;
			AudioConfig::getInstance()->playEffect("PICKITEM");
			GameObjectManager::getInstance()->getGameHub()->moveToBag(m_sprite, getPosition());
			addToPack();
		}
	}
}