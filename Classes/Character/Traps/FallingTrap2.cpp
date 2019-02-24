#include "FallingTrap2.h"
#include "Helper\Helper.h"

FallingTrap2::FallingTrap2()
{
	m_sprite = NULL;
	m_isFall = false;
}

void FallingTrap2::update(float deltaTime)
{
	Trap::update(deltaTime);

	if (m_isFall)
		m_physicsBody->SetType(b2_dynamicBody);
}

void FallingTrap2::collision(LivingObject *lObj, void *data)
{
	if (!m_isFall) {
		bool *p = &m_isFall;
		std::function<void(void)> func([p](){
			*p = true;
		});

		Helper::waitAndExecute(500, func);
	}
}

void FallingTrap2::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("Sprite") != attribute->MemberEnd()) {
		auto sprite = &(*attribute)["Sprite"];

		if (sprite->FindMember("Image") != sprite->MemberEnd()) {
			std::string link = (*sprite)["Image"].GetString();
			m_sprite = Sprite::create(link);
			m_sprite->retain();
		}

		if (sprite->FindMember("Scale") != sprite->MemberEnd())
			m_sprite->setScale((*sprite)["Scale"].GetFloat());
	}
}