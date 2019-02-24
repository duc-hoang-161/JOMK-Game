#include "Objects\DynamicGameObject.h"
#include "Helper\TraceImageHelper.h"

DynamicGameObject::DynamicGameObject(string direct) {
	string content = FileUtils::getInstance()
		->getStringFromFile(direct);
	rapidjson::Document document;
	document.Parse<0>(content.c_str());

	string plistDirect = document["Direct"].GetString();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistDirect);
	
	string defaultImageDirect = document["Default"].GetString();
	m_sprite = Sprite::createWithSpriteFrameName(defaultImageDirect);
	m_sprite->retain();
	
	m_physicsBodyManager = NULL;
	auto value = document.FindMember("PhysicsBodyDirect");
	if (value != document.MemberEnd())
		m_physicsBodyManager = new PhysicsBodyManager(document["PhysicsBodyDirect"].GetString());

	int tag = 1;
	for (rapidjson::Value::ConstMemberIterator it = document["Data"].MemberBegin();
		it != document["Data"].MemberEnd(); ++it)
	{
		Vector<SpriteFrame*> animFrames;

		string prefix = it->value["Prefix"].GetString();		// Get direct of image
		float delay = it->value["Delay"].GetFloat();		// Get delay
		int maxIndex = it->value["MaxIndex"].GetInt();
		int tag = it->value["Tag"].GetInt();

		for (int i = 1; i <= maxIndex; i++) {
			string dir = StringUtils::format("%s%d.png", prefix.c_str(), i);
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(dir);
			animFrames.pushBack(frame);

			if (m_physicsBodyManager != NULL) {
				if (defaultImageDirect == dir)
					m_physicsBodyManager->setDefaultFrame(frame);
				m_physicsBodyManager->add(frame, dir);
			}
		}

		// Create one action
 		auto animation = Animation::createWithSpriteFrames(animFrames, delay);
		//animation->setRestoreOriginalFrame(true);
		auto animate = Animate::create(animation);
		
		m_actions[tag] = Repeat::create(animate, 1);
		m_actions[tag]->retain();
		m_actions[tag]->setTag(tag);

		string actionName = it->name.GetString();
		m_actionNames[tag] = actionName;

		m_actionPriority[tag] = -1;
	}

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistDirect);
		// init variables
	m_countToUpdatePhysicsBody = 0;
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistDirect);
}

void DynamicGameObject::runAction(int actionTag)
{
	if (m_sprite->getNumberOfRunningActions() == 0)
		m_sprite->runAction(m_actions[actionTag]);
	else {
		if (m_sprite->getActionByTag(actionTag) == NULL) {
			int currentPriority = m_actionPriority[actionTag];

			bool isStopAll = true;
			for (auto it = m_actions.begin(); it != m_actions.end(); ++it) {
				int otherAction = it->first;
				if (m_sprite->getActionByTag(otherAction) != NULL 
					&& m_actionPriority[otherAction] > currentPriority) {
					isStopAll = false;
					break;
				}
				
			}

			if (isStopAll) {
				m_sprite->stopAllActions();
				m_sprite->runAction(m_actions[actionTag]);
			}
		}
	}
}

void DynamicGameObject::runActionWithCallBack(int actionTag, CallFunc *callBack)
{
	Sequence *seq = Sequence::create(m_actions[actionTag], callBack, nullptr);
	seq->setTag(actionTag);

	if (m_sprite->getNumberOfRunningActions() == 0)
		m_sprite->runAction(seq);
	else {
		if (m_sprite->getActionByTag(actionTag) == NULL) {
			int currentPriority = m_actionPriority[actionTag];

			bool isStopAll = true;
			for (auto it = m_actions.begin(); it != m_actions.end(); ++it) {
				int otherAction = it->first;
				if (m_sprite->getActionByTag(otherAction) != NULL
					&& m_actionPriority[otherAction] > currentPriority) {
					isStopAll = false;
					break;
				}

			}

			if (isStopAll) {
				m_sprite->stopAllActions();
				m_sprite->runAction(seq);
			}
		}
	}
}

bool DynamicGameObject::isRunningAction(int actionTag)
{
	return m_sprite->getActionByTag(actionTag) != NULL;
}

void DynamicGameObject::update(float deltaTime)
{
	if (m_physicsBodyManager != NULL) {
		if (m_countToUpdatePhysicsBody <= 1)
			m_countToUpdatePhysicsBody++;
		if (m_countToUpdatePhysicsBody > 1) {
			SpriteFrame *sf = m_sprite->getSpriteFrame();
			bool flipped = m_sprite->isFlippedX();

			if (sf != NULL) {
				if (sf != m_currentFrame) {
					b2Body *pb = m_physicsBodyManager->get(sf, m_sprite->isFlippedX());
					if (pb == NULL)
						if (flipped != m_flippedFrame) {
							sf = m_currentFrame;
							pb = m_physicsBodyManager->get(m_currentFrame, flipped);
						}
					if (pb != NULL) {
						if (m_physicsBody != NULL) {
							auto vec = m_physicsBody->GetLinearVelocity();
							pb->SetLinearVelocity(vec);
							pb->SetTransform(m_physicsBody->GetPosition(), 0);
							pb->SetType(m_physicsBody->GetType());
							m_physicsWorld->DestroyBody(m_physicsBody);
						}
						m_countToUpdatePhysicsBody = 0;

						m_physicsBody = pb;
						m_physicsBody->SetUserData((void*)this);

						m_currentFrame = sf;
						m_flippedFrame = flipped;
					}
				}
			}
			else
				if (m_physicsBody != NULL)
					m_physicsWorld->DestroyBody(m_physicsBody);
		}
	}

	GameObject::update(deltaTime);
}

void DynamicGameObject::createPhysicsBody(b2World *physicsWorld)
{
	if (m_physicsBodyManager == NULL)
		GameObject::createPhysicsBody(physicsWorld);
	else {
		m_physicsWorld = physicsWorld;
		m_physicsBodyManager->setPhysicsWorld(physicsWorld);

		m_physicsBody = m_physicsBodyManager->getDefault(m_sprite->isFlippedX());
		auto pos = b2Helper::asb2Vec2(getPosition());
		m_physicsBody->SetTransform(pos, 0);
		m_physicsBody->SetUserData(this);
	}
	
}

string DynamicGameObject::getActionName(int actionTag)
{
	return m_actionNames[actionTag];
}

void DynamicGameObject::setActionPriority(int actionTag, int priority) 
{
	m_actionPriority[actionTag] = priority;
}

DynamicGameObject::~DynamicGameObject()
{
	for (auto it = m_actions.begin(); it != m_actions.end(); ++it)
		it->second->release();
	delete m_physicsBodyManager;
}