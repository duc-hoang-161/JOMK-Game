#include "Character\Monsters\Monster.h"
#include "GameManager\GameObjectManager.h"
#include "Import\PackConfig.h"

const int Monster::REGION_BODY = 1;
const int Monster::REGION_DAMAGE = 2;

Monster::Monster(string animationFileDirect)
	: DynamicGameObject(animationFileDirect)
{
	pRegisterPhysicsBody(&m_physicsBody);
	pRegisterHP(&m_HP);
	pRegisterMaxHP(&m_maxHP);
	pRegisterSprite(&m_sprite);

	m_HP = m_maxHP = 1;

	m_filterData.categoryBits = BITMASK_MONSTER;
	m_filterData.maskBits = -1;

	// Hiển thị thanh máu
	m_healthBarBackground = Sprite::create("opp-assets/Hpmoster_bg.png");
	m_healthBar = ui::LoadingBar::create("opp-assets/Hpmoster.png");
	m_healthBar->setPercent(0);
	m_healthBar->setDirection(ui::LoadingBar::Direction::LEFT);	
	cocos2d::Vec2 scaleRatio(getActualSize().width / m_healthBarBackground->getContentSize().width,
		getActualSize().width / m_healthBarBackground->getContentSize().width * 2);	
	m_healthBarBackground->setScaleX(scaleRatio.x);
	m_healthBarBackground->setScaleY(scaleRatio.y);
	m_healthBar->setScaleX(scaleRatio.x);
	m_healthBar->setScaleY(scaleRatio.y);
	
	m_healthBarBackground->retain();
	m_healthBar->retain();

	m_isAlive = true;
}

void Monster::addParent(Layer *layer)
{
	DynamicGameObject::addParent(layer, 1);	// super
	m_parent->addChild(m_healthBarBackground, 1);
	m_parent->addChild(m_healthBar, 1);

	for (int i = 0; i < m_items.size(); i++)
		m_items[i]->addParent(layer, 1);
}

void Monster::update(float deltatTime)
{
	for (int i = 0; i < m_items.size(); i++)
		if (true)
			m_items[i]->update(deltatTime);

	if (!m_isAlive)
		return;

	DynamicGameObject::update(deltatTime);
	
	// update hp
	Vec2 position = getPosition();
	m_healthBarBackground->setPosition(Vec2(position.x, position.y +
		(getActualSize().height / 2 + m_healthBarBackground->getContentSize().height / 2)));
	m_healthBar->setPosition(m_healthBarBackground->getPosition());
	updateHealthBar();

	if (m_HP <= 0)
		this->destroy();
}

void Monster::showGold()
{
	if (m_sprite != NULL) {
		auto scene = m_sprite->getParent();

		Node *node = Node::create();

		std::string c = cocos2d::StringUtils::format("+ %d", m_gold);
		auto myLabel = cocos2d::Label::createWithTTF(c, "fonts/Marker Felt.ttf", 30);
		myLabel->setColor(cocos2d::Color3B(0, 255, 0));
		myLabel->setAnchorPoint(Vec2(0, 0.5));

		node->addChild(myLabel);

		cocos2d::Vec2 delta;
		auto size = m_sprite->getContentSize();
		auto pos = m_sprite->getPosition();
		delta.x = (1 - m_sprite->getAnchorPoint().x) * size.width + 20;
		delta.y = (1 - m_sprite->getAnchorPoint().y) * size.height;
		pos.x += delta.x, pos.y += delta.y;
		node->setAnchorPoint(Vec2(0, 0.5));
		node->setPosition(pos);

		cocos2d::Sprite *coin = GameObjectManager::getInstance()->nextCoin();
		if (coin != NULL) {
			coin->setVisible(true);
			coin->setAnchorPoint(Vec2(0, 0.5));

			myLabel->setPosition(Vec2(0, node->getContentSize().height / 2));
			coin->setPosition(Vec2(myLabel->getContentSize().width + 5, node->getContentSize().height / 2));

			node->addChild(coin);
			scene->addChild(node);

			cocos2d::Sequence *s = cocos2d::Sequence::create(
				cocos2d::Spawn::create(
				cocos2d::MoveBy::create(1, cocos2d::Vec3(0, 50, 0)),
				cocos2d::FadeOut::create(1),
				nullptr
				),
				cocos2d::CallFunc::create([scene, coin, node]() {
					node->removeChild(coin);
					scene->removeChild(node);
					coin->setOpacity(255);
					coin->setVisible(false);

					GameObjectManager::getInstance()->getGameHub()->moveToCoins(coin, node->getPosition());
				}),
				nullptr
			);

			node->runAction(s);
		}
	}
}

void Monster::setAttribute(string jsonFile)
{
	string content = FileUtils::getInstance()
		->getStringFromFile(jsonFile);
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	setAttribute(&document);
}

void Monster::setAttribute(const rapidjson::GenericValue < rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator
	< rapidjson::CrtAllocator >> *attribute)
{
	if (attribute->FindMember("MaxHP") != attribute->MemberEnd())
		m_HP = m_maxHP = (*attribute)["MaxHP"].GetInt();
	if (attribute->FindMember("Damage") != attribute->MemberEnd())
		m_damage = (*attribute)["Damage"].GetInt();
	if (attribute->FindMember("RunStep") != attribute->MemberEnd())
		m_runStep = (*attribute)["RunStep"].GetInt();
	if (attribute->FindMember("Gold") != attribute->MemberEnd())
		m_gold = (*attribute)["Gold"].GetInt();
	if (attribute->FindMember("Items") != attribute->MemberEnd()) {
		auto arr = (*attribute)["Items"].GetArray();
		for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
			auto item = arr[i].GetArray();
			auto type = item[0].GetInt();
			auto id = item[1].GetInt();

			Item *it = new Item(type, id);
			m_items.push_back(it);
		}
		Vec2 rdItem = ItemsConfig::getInstance()->getRandomItem();
		Item* itemRand = new Item(rdItem.x, rdItem.y);
		m_items.push_back(itemRand);
	}
}

void Monster::addMainCharacter(MainCharacter *mc)
{
	m_mainCharacter = mc;
}

void Monster::createPhysicsBody(b2World *physicsWorld)
{
	DynamicGameObject::createPhysicsBody(physicsWorld);
	b2Fixture *fixture = m_physicsBody->GetFixtureList();
	while (fixture->GetNext() != NULL) {
		fixture->SetFilterData(m_filterData);
		fixture = fixture->GetNext();
	}
	m_physicsBody->SetUserData((void*)this);
	m_physicsBody->SetBullet(true);

	for (int i = 0; i < m_items.size(); i++)
		m_items[i]->createPhysicsBody(physicsWorld);
}

void Monster::updateHealthBar()
{
	if (m_maxHP != 0)
		m_healthBar->setPercent(m_HP * 1.0 / m_maxHP * 100);
}

void Monster::moveLeft(float deltaTime)
{
	if (m_physicsBody != NULL) {
		if (m_sprite != NULL)
			m_sprite->setFlippedX(true);
		auto pos = m_physicsBody->GetPosition();
		pos.x -= b2Helper::asb2Value(m_runStep * RATIO_RUN_STEP * deltaTime);
		m_physicsBody->SetTransform(pos, 0);
	}
}

void Monster::addItem(Item *item)
{
	m_items.push_back(item);
}

void Monster::moveRight(float deltaTime)
{
	if (m_physicsBody != NULL) {
		if (m_sprite != NULL)
			m_sprite->setFlippedX(false);
		auto pos = m_physicsBody->GetPosition();
		pos.x += b2Helper::asb2Value(m_runStep * RATIO_RUN_STEP * deltaTime);
		m_physicsBody->SetTransform(pos, 0);
	}
}

void Monster::destroy()
{
	if (m_isAlive) {
		m_isAlive = false;
		setVisible(false);
		removePhysicsBody();
		m_healthBar->setVisible(false);
		m_healthBarBackground->setVisible(false);

		PackConfig::getInstance()->addCoins(m_gold);
		showGold();

		Vec2 centerPosition = getPosition();
		int sign = (m_mainCharacter->getPosition().x < centerPosition.x) ? 1 : -1;
		for (int i = 0; i < m_items.size(); i++)
			m_items[i]->drop(Vec2(centerPosition.x + sign * i * m_items[i]->getActualSize().width, centerPosition.y + 100));
	}
}

bool Monster::attack(LivingObject *lObj)
{
	auto other = lObj->pGetPosition();
	auto mine = m_physicsBody->GetPosition();
	
	return false;
}

Monster::~Monster()
{
	m_healthBarBackground->release();
	m_healthBar->release();
}
