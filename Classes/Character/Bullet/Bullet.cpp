#include "Bullet.h"

const int Bullet::STATE_RUN = 1;

Bullet::Bullet(const std::string &animationFileDirect) : 
	DynamicGameObject(animationFileDirect)
{
	m_filterData.categoryBits = BITMASK_BULLET;
	m_filterData.maskBits = -1;
	if (m_physicsBodyManager != NULL)
		m_physicsBodyManager->addFilter(1, m_filterData);
	m_sprite->setVisible(false);
	m_maxDistance = 500;
	m_maximumTime = -1;

	setAlive(false);

	m_animateForever = false;
}

void Bullet::addCollisionCondition(int bitmask, 
		const std::function<void(Bullet*, LivingObject*)> &func)
{
	m_collision[bitmask] = func;
}

void Bullet::createPhysicsBody(b2World *physicsWorld)
{
	m_physicsWorld = physicsWorld;
	if (m_physicsBodyManager != NULL)
		m_physicsBodyManager->setPhysicsWorld(m_physicsWorld);
}

void Bullet::setDirect(const cocos2d::Vec2 &direct)
{
	m_direct = direct;
}

void Bullet::setMaxDistance(int maxDistance)
{
	m_maxDistance = maxDistance;
}

void Bullet::collision(int bitmask, LivingObject *lObj)
{
	if (m_collision.find(bitmask) != m_collision.end()) {
		if (lObj != NULL && isAlive()) {
			if (m_attackedObject.find(lObj) == m_attackedObject.end())
				(m_collision[bitmask])(this, lObj),
				m_attackedObject.insert(lObj);
		}
	}
}

void Bullet::setFilterData(b2Filter filter)
{
	DynamicGameObject::setFilterData(filter);
	if (m_physicsBodyManager != NULL)
		m_physicsBodyManager->addFilter(1, m_filterData);
}

void Bullet::run(const cocos2d::Vec2 &position, const cocos2d::Vec2 &direct)
{
	m_sprite->setFlippedX(direct.x < 0);

	m_physicsBody = m_physicsBodyManager->getDefault(m_sprite->isFlippedX());
	m_physicsBody->SetType(b2_staticBody);
	m_physicsBody->SetUserData((void*)this);
	runAction(STATE_RUN);

	setPosition(position);
	setDirect(direct);
	m_isDestroy = false;
	setAlive(true);
	m_attackedObject.clear();
	m_sprite->setVisible(true);

	m_startPosition = position;
	t_startTime = clock();
}

void Bullet::addParent(Layer *layer)
{
	layer->addChild(m_sprite, 214);
}

void Bullet::update(float deltaTime)
{
	// update position
	auto pos = m_physicsBody->GetPosition();
	pos.x += b2Helper::asb2Value(m_direct.x * deltaTime);
	pos.y += b2Helper::asb2Value(m_direct.y * deltaTime);
	m_physicsBody->SetTransform(pos, 0);

	DynamicGameObject::update(deltaTime);

	if (!isRunningAction(STATE_RUN) && m_animateForever)
		runAction(STATE_RUN);

	auto curPos = getPosition();
	if (isAlive() && 
			(m_startPosition.distance(curPos) > m_maxDistance 
			|| m_isDestroy 
			|| (m_maximumTime > 0 && ((int)(clock() - t_startTime)) > m_maximumTime))) {
		setAlive(false);
		m_sprite->setVisible(false);
		removePhysicsBody();
	}
}

void Bullet::setTime(int ms)
{
	m_maximumTime = ms;
}

void Bullet::setUserData(void *data)
{
	m_userData = data;
}

void Bullet::setAnimateForever(bool v)
{
	m_animateForever = v;
}

void Bullet::setAlive(bool v)
{
	m_isAlive = v;
}

bool Bullet::isAlive()
{
	return m_isAlive;
}

void Bullet::destroy()
{
	m_isDestroy = true;
}