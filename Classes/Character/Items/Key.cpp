#include "Key.h"
#include "GameManager\GameObjectManager.h"

Key::Key()
{
	m_sprite = Sprite::create("items/key/key.png");
	m_sprite->retain();

	m_sprite->setVisible(false);
	m_isAlive = true;
}

void Key::addToPack()
{
	GameObjectManager::getInstance()->getGameHub()->setVisibleKey(true);
}