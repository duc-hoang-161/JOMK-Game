#include "Character\Monsters\BatMonster.h"


BatMonster::BatMonster()
{
	m_sprite = NULL;
}
void BatMonster::collision(LivingObject *lObj, void *data)
{
	if (canAttack(lObj, 0)) {
		addAttackedObject(lObj, 0);
		eraseAttackedObjectAfter(lObj, 0, 1000);
		lObj->pReceiveDamage(m_damage);
	}
}

void BatMonster::update(float deltaTime)
{
	if (m_sprite != NULL) {
		Vec2 pos = m_sprite->getPosition();
		m_physicsBody->SetTransform(b2Helper::asb2Vec2(pos), 0);
	}
}

void BatMonster::createPhysicsBody(b2World *physicsWorld)
{
	Trap::createPhysicsBody(physicsWorld);
}

void BatMonster::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("Sprite") != attribute->MemberEnd()) {
		auto sprite = &(*attribute)["Sprite"];

		if (sprite->FindMember("Image") != sprite->MemberEnd()) {
			string link = (*sprite)["Image"].GetString();
			m_sprite = Sprite::create(link);
			m_sprite->retain();

			if (sprite->FindMember("Scale") != sprite->MemberEnd())
				m_sprite->setScale((*sprite)["Scale"].GetFloat());
			if (attribute->FindMember("Damage") != attribute->MemberEnd())
				m_damage = (*attribute)["Damage"].GetInt();
		}
		if (attribute->FindMember("Distance") != attribute->MemberEnd()) {
			m_distance = (*attribute)["Distance"].GetInt();
			BatMonster::moveForever();
		}
		
	}
}
 void BatMonster::moveForever()
{
	if (m_sprite != NULL){
	// create a MoveBy Action to where we want the sprite to drop from.
		auto move_1 = MoveTo::create(2, Vec2(m_distance, m_distance));
		auto move_2 = MoveTo::create(2, Vec2(m_distance * 2 / 3, m_distance*2));
		auto move_3 = MoveTo::create(2, Vec2(m_distance * 2, m_distance));
	
	// create a BounceIn Ease Action
	auto move_ease_in = EaseBounceIn::create(move_2->clone());

	// create a delay that is run in between sequence events
	auto delay = DelayTime::create(0.25f);

	// create the sequence of actions, in the order we want to run them
	auto seq1 = Sequence::create(move_1, delay, move_ease_in,
		delay->clone(),move_3, nullptr);
	

	// run the sequence and repeat forever.
	m_sprite->runAction(RepeatForever::create(seq1));
	}
}
