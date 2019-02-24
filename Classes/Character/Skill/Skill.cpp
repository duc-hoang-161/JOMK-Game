#include "Skill.h"

Skill::Skill(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	m_quantity = (*attribute)["Count"].GetInt();
	m_IDs.resize(m_quantity);
	m_manaToUses.resize(m_quantity);
	m_times.resize(m_quantity);

	// set id
	auto ids = (*attribute)["ID"].GetArray();
	for (auto i = 0; i < ids.Size(); i++) {
		auto id = ids[i].GetArray();
		m_IDs[i].set(id[0].GetInt(), id[1].GetInt());
	}

	// set time
	auto times = (*attribute)["Attribute"]["Time"].GetArray();
	for (auto i = 0; i < times.Size(); i++)
		m_times[i] = times[i].GetInt();

	// set mana to use
	auto manaToUses = (*attribute)["Attribute"]["Mana"].GetArray();
	for (auto i = 0; i < manaToUses.Size(); i++)
		m_manaToUses[i] = manaToUses[i].GetInt();

	m_level = 0;
	t_startTime = 0;
	m_isRunning = false;
}

void Skill::setMainCharacter(MainCharacter *mc)
{
	m_mainCharacter = mc;
}

int Skill::getRestTime()
{
	if (t_startTime == 0)
		return 0;
	if (m_times[m_level] == -1)
		return 0;
	return max(0, m_times[m_level] * 1000 - (int)(clock() - t_startTime));
}

bool Skill::isRunning()
{
	return m_isRunning;
}

void Skill::setLevel(int level)
{
	level = max(level, m_quantity - 1);
	m_level = level;
}

int Skill::getTime()
{
	return m_times[m_level];
}

bool Skill::execute()
{
		// nếu không đủ năng lượng
	if (m_mainCharacter->getEnergy() < m_manaToUses[m_level])
		return false;

		// nếu chưa hồi chiêu
	if (m_isRunning)
		return false;

	m_mainCharacter->subEnergy(m_manaToUses[m_level]);
	
	m_isRunning = true;
	Helper::waitAndExecute(m_times[m_level] * 1000, [&]() {
		m_isRunning = false;
	});

	std::string c = (m_manaToUses[m_level] == 0) ? "" : cocos2d::StringUtils::format("-%d", m_manaToUses[m_level]);
	auto myLabel = cocos2d::Label::createWithTTF(c, "fonts/Marker Felt.ttf", 30);
	myLabel->setColor(cocos2d::Color3B(0, 0, 255));

	auto pos = m_mainCharacter->getPosition();
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