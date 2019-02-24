#include "Bullets.h"

Bullets::Bullets(int number, const std::string &file)
{
	for (int i = 0; i < number; i++)
		m_bullets.push_back(new Bullet(file));
}

bool Bullets::nextBullet(const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct)
{
	for (int i = 0; i < m_bullets.size(); i++) {
		if (!m_bullets[i]->isAlive()) {
			m_bullets[i]->run(position, direct);
			return true;
		}
	}
	return false;
}

void Bullets::addCollisionCondition(int bitmask, const std::function<void(Bullet*, LivingObject*)> &func)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->addCollisionCondition(bitmask, func);
}

void Bullets::setMaxDistance(int maxDistance)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->setMaxDistance(maxDistance);
}

void Bullets::createPhysicsBody(b2World *physicsWorld)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->createPhysicsBody(physicsWorld);
}

void Bullets::addParent(Layer *layer)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->addParent(layer);
}

void Bullets::setFilterData(b2Filter filter)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->setFilterData(filter);
}

void Bullets::setAnchorPoint(Vec2 vec2)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->setAnchorPoint(vec2);
}

void Bullets::setUserData(void *data)
{

}

void Bullets::setAnimateForever(bool v)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->setAnimateForever(v);
}

void Bullets::setTime(int ms)
{
	for (int i = 0; i < m_bullets.size(); i++)
		m_bullets[i]->setTime(ms);
}

void Bullets::update(float deltaTime)
{
	for (int i = 0; i < m_bullets.size(); i++)
		if (m_bullets[i]->isAlive())
			m_bullets[i]->update(deltaTime);
}

Bullets::~Bullets()
{
	for (int i = 0; i < m_bullets.size(); i++)
		delete m_bullets[i];
	m_bullets.clear();
}