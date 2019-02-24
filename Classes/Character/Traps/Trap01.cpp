#include"Character/Traps/Trap01.h"
#include"Objects/GameObject.h"

Trap01::Trap01(string linkImg)
{
	m_sprite = Sprite::create(linkImg);
	
}
void Trap01::update(float datatime)
{
	Vec2 pos = m_sprite->getPosition();
	float32 a = -1 * CC_DEGREES_TO_RADIANS(m_sprite->getRotation());
	m_physicsBody->SetTransform(b2Helper::asb2Vec2(pos), a);
}
void Trap01::createPhysicBody(b2World *physicsWorld){
	/*GameObject::createPhysicsBody(physicsWorld);
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	m_physicsBody->SetType(b2_staticBody);*/
	auto pos = b2Helper::asb2Vec2(getPosition());
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(pos.x, pos.y);
	myBodyDef.angle = 0;
	myBodyDef.bullet = true;

	m_physicsBody = physicsWorld->CreateBody(&myBodyDef);

	Size size = getActualSize();
	b2PolygonShape shape;
	shape.SetAsBox(b2Helper::asb2Value(size.width*0.5f), b2Helper::asb2Value(size.height*0.5f));

	// Khai báo fixtureDef;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &shape;
	myFixtureDef.friction = 1;

	m_physicsBody->CreateFixture(&myFixtureDef); //thêm một fixture vào body

	m_physicsWorld = physicsWorld;
	
}

void Trap01::setVisble(bool v){
	m_sprite->setVisible(v);
}

void Trap01::move()
{
	m_sprite->setAnchorPoint(Vec2(0.5f, 1.0f));
	auto move = Sequence::createWithTwoActions(RotateTo::create(2.0f, 40.0f), RotateTo::create(2.0f, -40.0f));
	m_sprite->runAction(RepeatForever::create(move));
}