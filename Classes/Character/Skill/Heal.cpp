#include "Heal.h"

Heal::Heal(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute) : Skill(attribute)
{
	auto buffHPs = (*attribute)["Attribute"]["BuffHP"].GetArray();

	for (auto i = 0; i < buffHPs.Size(); i++)
		m_buffHPs.push_back(buffHPs[i].GetInt());
}

bool Heal::execute()
{
	if (!Skill::execute())
		return false;

	m_mainCharacter->addHP(m_buffHPs[m_level]);

	std::string c = (m_buffHPs[m_level] == 0) ? "" : cocos2d::StringUtils::format("+%d", m_buffHPs[m_level]);
	auto myLabel = cocos2d::Label::createWithTTF(c, "fonts/Marker Felt.ttf", 30);
	myLabel->setColor(cocos2d::Color3B(0, 255, 0));

	auto pos = m_mainCharacter->getPosition();
	pos.x += m_mainCharacter->getContentSize().width / 2 + 20;
	pos.y += m_mainCharacter->getContentSize().height + 20;

	myLabel->setPosition(pos);
	auto parent = m_mainCharacter->getParent();
	parent->addChild(myLabel);

	cocos2d::Sequence *s = cocos2d::Sequence::create(
		cocos2d::Spawn::create(
		cocos2d::MoveBy::create(1, cocos2d::Vec3(0, 30, 0)),
		cocos2d::FadeOut::create(1),
		nullptr
		),
		cocos2d::CallFunc::create([parent, myLabel]() {
		parent->removeChild(myLabel);
	}),
		nullptr);

	myLabel->runAction(s);

	return true;
}