#include"Character/Traps/TrapMoveDown.h"
#include"Objects/GameObject.h"
TrapMoveDown::TrapMoveDown(string linkImg)
{
	m_sprite = Sprite::create(linkImg);

}
void TrapMoveDown::update(float datatime)
{
	GameObject::update(datatime);
	Vec2 pos = m_sprite->getPosition();
	
	m_physicsBody->SetTransform(b2Helper::asb2Vec2(pos), 0);
}
void TrapMoveDown::createPhysicBody(b2World *physicsWorld){

	auto pos = b2Helper::asb2Vec2(getPosition());
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(pos.x, pos.y);
	myBodyDef.angle = 0;
	myBodyDef.bullet = false;

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

