#include "RockFall.h"

RockFall::RockFall(std::string file)
{
	m_sprite = Sprite::create(file);
}

void RockFall::update(float deltaTime)
{
	GameObject::update(deltaTime);
}

void RockFall::Fall()
{
	m_physicsBody->SetType(b2_dynamicBody);
}

void RockFall::createPhysicsBody(b2World *physicsWorld)
{
	/*GameObject::createPhysicsBody(physicsWorld);
	m_physicsBody->SetType(b2_staticBody);*/

	auto pos = b2Helper::asb2Vec2(getPosition());
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(pos.x, pos.y);
	myBodyDef.angle = 0;

	m_physicsBody = physicsWorld->CreateBody(&myBodyDef);
	Size size = getActualSize();
	size.width = min(size.width, (float)400);
	size.height = min(size.height, (float)128);
	b2PolygonShape shape;
	shape.SetAsBox(b2Helper::asb2Value(size.width*0.5f), b2Helper::asb2Value(size.height*0.5f));

	// Khai báo fixtureDef;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &shape;
	myFixtureDef.friction = 1;

	m_physicsBody->CreateFixture(&myFixtureDef); //thêm một fixture vào body
	m_physicsWorld = physicsWorld;
}