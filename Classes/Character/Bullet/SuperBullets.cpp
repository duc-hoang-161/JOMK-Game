#include "SuperBullets.h"

SuperBullets::SuperBullets()
{

}

void SuperBullets::addBullets(int key, Bullets *bullets)
{
	m_bullets[key] = bullets;
}

void SuperBullets::addParent(cocos2d::Layer *layer)
{
	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
		it->second->addParent(layer);
}

Bullets* SuperBullets::getBullets(int key)
{
	if (m_bullets.find(key) != m_bullets.end())
		return m_bullets[key];
	return NULL;
}

void SuperBullets::createPhysicsBody(b2World *world)
{
	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
		it->second->createPhysicsBody(world);
}

void SuperBullets::update(float deltaTime)
{
	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
		it->second->update(deltaTime);
}

void SuperBullets::nextBullet(int key, const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct)
{
	if (m_bullets.find(key) != m_bullets.end())
		m_bullets[key]->nextBullet(position, direct);
}