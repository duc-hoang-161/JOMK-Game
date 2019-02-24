#include "Objects\GameObject.h"
#include "Helper\MyBodyParser.h"

int GameObject::COUNT = 1;

GameObject::GameObject() {
	m_tag = COUNT++;
	m_sprite = NULL;
	m_physicsBody = NULL;
	m_isAlive = true;
}

bool GameObject::isIntersect(GameObject *p) {
	return m_sprite->getBoundingBox().intersectsRect(p->m_sprite->getBoundingBox());
}

void GameObject::addParent(Layer *parent)
{
	parent->addChild(m_sprite);
	m_parent = parent;
}

void GameObject::addParent(Layer *parent, int z)
{
	parent->addChild(m_sprite, z);
	m_parent = parent;
}

void GameObject::createPhysicsBody(b2World *physicsWorld)
{
	auto pos = b2Helper::asb2Vec2(getPosition());
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(pos.x,pos.y);
	myBodyDef.angle = 0;

	m_physicsBody = physicsWorld->CreateBody(&myBodyDef);

	Size size = getActualSize();
	b2PolygonShape shape;
	shape.SetAsBox(b2Helper::asb2Value(size.width / 2), b2Helper::asb2Value(size.height / 2));

	// Khai báo fixtureDef;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &shape;
	myFixtureDef.friction = 1;
	myFixtureDef.filter = m_filterData;

	m_physicsBody->CreateFixture(&myFixtureDef); //thêm một fixture vào body
	m_physicsBody->SetUserData(this);

	m_physicsWorld = physicsWorld;
}

void GameObject::createPhysicsBody(b2World *physicsWorld, std::string data, std::string config, std::string key)
{
	auto pos = b2Helper::asb2Vec2(getPosition());
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(pos.x, pos.y);
	myBodyDef.angle = 0;
	myBodyDef.bullet = true;

	m_physicsBody = physicsWorld->CreateBody(&myBodyDef);

	MyBodyParser::getInstance()->parseJsonFile(data, config);
	auto fixtureDef = MyBodyParser::getInstance()->bodyFormJson(key).first;
	while (fixtureDef->next) {
		b2Fixture *fixture = m_physicsBody->CreateFixture(&fixtureDef->fixture);
		fixture->SetFilterData(m_filterData);
		fixtureDef = fixtureDef->next;
	}

	m_physicsBody->SetType(b2_staticBody);
	m_physicsBody->SetUserData(this);
	m_physicsWorld = physicsWorld;
}

int GameObject::getTag()
{
	return m_tag;
}

void GameObject::removePhysicsBody()
{
	m_physicsWorld->DestroyBody(m_physicsBody);
}

Layer* GameObject::getParent()
{
	return m_parent;
}

void GameObject::setPosition(Vec2 position)
{
	b2Vec2 vec = b2Helper::asb2Vec2(position);
	if (m_physicsBody != NULL)
		m_physicsBody->SetTransform(vec, 0);
	else
		m_sprite->setPosition(position);
}

Vec2 GameObject::getPosition()
{
	return m_sprite->getPosition();
}

Size GameObject::getContentSize()
{
	return m_sprite->getContentSize();
}

b2Body* GameObject::getPhysicsBody()
{
	return m_physicsBody;
}

Size GameObject::getActualSize()	// after scale
{
	Size size = m_sprite->getContentSize();
	Vec2 scale = ccp(m_sprite->getScaleX(), m_sprite->getScaleY());

	return Size(size.width * scale.x, size.height*scale.y);
}

void GameObject::update(float deltaTime)
{
	b2Vec2 pos = m_physicsBody->GetPosition();
	m_sprite->setPosition(b2Helper::asVec2(pos));
}

void GameObject::setAnchorPoint(Vec2 vec2)
{
	m_sprite->setAnchorPoint(vec2);
}

bool GameObject::isVisible()
{
	return m_sprite->isVisible();
}

void GameObject::setVisible(bool v)
{
	m_sprite->setVisible(v);
}

bool GameObject::isAlive()
{
	return m_isAlive;
}

void GameObject::setFilterData(b2Filter filter)
{
	m_filterData = filter;
}

GameObject::~GameObject()
{
	if (m_sprite != NULL)
		m_sprite->release();
}
