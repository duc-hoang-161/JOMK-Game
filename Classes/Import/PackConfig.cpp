#include "PackConfig.h"

PackConfig::PackConfig(){};

PackConfig* PackConfig::packConfig = NULL;

PackConfig* PackConfig::getInstance()
{
	if (packConfig == NULL)
	{
		packConfig = new PackConfig();
		packConfig->MAX_SLOT = 0;
		packConfig->expandSlotStep = 0;
		packConfig->availableSlot = 0;
		packConfig->coins = 0;
		packConfig->magicPoint = 0;
		packConfig->passedPoint = 0;
		packConfig->readConfig();
	}
	return packConfig;
}

std::string PackConfig::JsonToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}

void PackConfig::readConfig()
{
	auto sharedFileUtils = FileUtils::getInstance();
	static std::string data = sharedFileUtils->getStringFromFile(FileUtils::getInstance()->getWritablePath() + pathConfigFile);
	if (data.length() < 5)
	{
		data = sharedFileUtils->getStringFromFile(pathConfigFile);
		CCLOG("%s", data.c_str());
		std::fstream f;
		std::string path = FileUtils::getInstance()->getWritablePath() + pathConfigFile;
		f.open(path.c_str(), std::ios::out);
		if (f.fail())
		{
			CCLOG("can not create file %s", path.c_str());
			return;
		}
		else
		{
			f.close();
			FileUtils::getInstance()->writeStringToFile(data, path);
		}
	}
	packConfig->document.Parse<0>(data.c_str());
	CCLOG("%s", data.c_str());
	if (packConfig->document.HasParseError())
	{
		log("LOI PASRE JSON");
	}
	else
	{
		if (packConfig->document.IsObject())
		{
			assert(packConfig->document.IsObject());
			if (packConfig->document.HasMember("pack"))
			{
				rapidjson::Value& valuePack = packConfig->document["pack"];
				if (valuePack.IsObject())
				{
					this->MAX_SLOT = cocos2d::Value(valuePack["MAXSLOT"].GetInt()).asInt();
					priceUpgrade = cocos2d::Value(valuePack["priceUpgrade"].GetInt()).asInt();
					expandSlotStep = cocos2d::Value(valuePack["expandSlotStep"].GetInt()).asInt();
					availableSlot = cocos2d::Value(valuePack["availableSlot"].GetInt()).asInt();
					coins = cocos2d::Value(valuePack["coins"].GetInt()).asInt();
					magicPoint = cocos2d::Value(valuePack["magicPoint"].GetInt()).asInt();
					passedPoint = cocos2d::Value(valuePack["passedPoint"].GetInt()).asInt();
				}
			}
			
			rapidjson::Value& valueItems = packConfig->document["items"];
			if (valueItems.IsArray())
			{
				for (int i = 0; i < valueItems.Size(); i++)
				{
					rapidjson::Value& v = valueItems[i];
					assert(v.IsArray());
					Vec2 vec(cocos2d::Value(v[0].GetInt()).asInt(), cocos2d::Value(v[1].GetInt()).asInt());
					listItems.push_back(vec);
				}
			}
			rapidjson::Value& valueSkills = packConfig->document["skillLvl"];
			if (valueSkills.IsArray())
			{
				for (int i = 0; i < valueSkills.Size(); i++)
				{
					rapidjson::Value& v = valueSkills[i];
					assert(v.IsArray());
					Vec2 vec(cocos2d::Value(v[0].GetInt()).asInt(), cocos2d::Value(v[1].GetInt()).asInt());
					listSkillLevel.push_back(vec);
				}
			}
			rapidjson::Value& valueEquipItems = packConfig->document["equip"];
			if (valueEquipItems.IsArray())
			{
				for (int i = 0; i < valueEquipItems.Size(); i++)
				{
					rapidjson::Value& v = valueEquipItems[i];
					assert(v.IsArray());
					Vec3 vec(cocos2d::Value(v[0].GetInt()).asInt(), cocos2d::Value(v[1].GetInt()).asInt(), cocos2d::Value(v[2].GetInt()).asInt());
					listEquipItems.push_back(vec);
				}
			}
		}
	}
}

int PackConfig::getTotalSlot()
{
	return this->MAX_SLOT;
}
int PackConfig::getAvailableSlot()
{
	return this->availableSlot;
}
int PackConfig::getExpandStep()
{
	return this->expandSlotStep;
}
int PackConfig::getCoins()
{
	return this->coins;
}
int PackConfig::getMagicPoint()
{
	return this->magicPoint;
}
int PackConfig::getPassedPoint()
{
	return this->passedPoint;
}
bool PackConfig::passNewPoint(int level)
{
	if (level == passedPoint)
	{
		this->passedPoint++;
		updateConfig();
		return true;
	}
	return false;
}
std::vector<Vec2> PackConfig::getItemsPack()
{
	return this->listItems;
}

void PackConfig::addCoins(int number)
{
	this->coins += number;
	this->updateConfig();
}

void PackConfig::subCoins(int number)
{
	if (coins - number >= 0)
	{
		this->coins -= number;
		this->updateConfig();
	}
	
}

int PackConfig::getPriceUpgrade()
{
	return this->priceUpgrade;
}

void PackConfig::updateConfig()
{
	if (document.HasMember("pack"))
	{
		rapidjson::Value& value = document["pack"];
		assert(value.IsObject());
		if (value.HasMember("availableSlot") && value.HasMember("coins") && value.HasMember("magicPoint"))
		{
			value["availableSlot"].SetInt(this->availableSlot);
			value["coins"].SetInt(this->coins);
			value["magicPoint"].SetInt(this->magicPoint);
			value["passedPoint"].SetInt(this->passedPoint);
		}
	}
	if (document.HasMember("items"))
	{
		rapidjson::Value value(rapidjson::Type::kArrayType);
		value.SetArray();
		assert(value.IsArray());
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		for (int i = 0; i < this->listItems.size(); i++)
		{
			rapidjson::Value v(rapidjson::Type::kArrayType);
			v.PushBack((int)listItems[i].x, allocator);
			v.PushBack((int)listItems[i].y, allocator);
			value.PushBack(v, allocator);
		}
		document["items"] = value;
	}
	if (document.HasMember("skillLvl"))
	{
		rapidjson::Value value(rapidjson::Type::kArrayType);
		value.SetArray();
		assert(value.IsArray());
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		for (int i = 0; i < this->listSkillLevel.size(); i++)
		{
			rapidjson::Value v(rapidjson::Type::kArrayType);
			v.PushBack((int)listSkillLevel[i].x, allocator);
			v.PushBack((int)listSkillLevel[i].y, allocator);
			value.PushBack(v, allocator);
		}
		document["skillLvl"] = value;
	}
	if (document.HasMember("equip"))
	{
		rapidjson::Value value(rapidjson::Type::kArrayType);
		value.SetArray();
		assert(value.IsArray());
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		for (int i = 0; i < this->listEquipItems.size(); i++)
		{
			rapidjson::Value v(rapidjson::Type::kArrayType);
			v.PushBack((int)listEquipItems[i].x, allocator);
			v.PushBack((int)listEquipItems[i].y, allocator);
			v.PushBack((int)listEquipItems[i].z, allocator);
			value.PushBack(v, allocator);
		}
		document["equip"] = value;
	}
	FileUtils::getInstance()->writeStringToFile(this->JsonToString(document), FileUtils::getInstance()->getWritablePath() + this->pathConfigFile);
	
}

void PackConfig::addItem(Vec2 newItem)
{
	if (this->listItems.size() < this->availableSlot)
	{
		int i = 0;
		for (i = 0; i < this->listItems.size(); i++)
		{
			if (newItem.x > this->listItems[i].x)
				continue;
			if (newItem.x == this->listItems[i].x)
			{
				if (newItem.y > this->listItems[i].y)
				{
					continue;
				}
				else
				{
					break;
				}
			}
			if (newItem.x < this->listItems[i].x)
			{
				break;
			}
		}
		this->listItems.insert(this->listItems.begin() + i, newItem);
		this->updateConfig();
		for (int j = 0; j < 5; j++)
		{
			if (this->listEquipItems[j].x >= i)
				this->listEquipItems[j].x++;
		}
	}
}

void PackConfig::addItem(int idType, int idItem)
{
	addItem(Vec2(idType, idItem));
}

void PackConfig::expandPackSize()
{
	if (this->availableSlot < this->MAX_SLOT)
	{
		this->availableSlot += this->expandSlotStep;
		this->updateConfig();
	}
}

void PackConfig::removeItem(Vec2 item)
{
	int i = 0;
	for (i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == item.x && this->listItems[i].y == item.y)
		{
			this->listItems.erase(this->listItems.begin() + i);
			break;
		}
	}
	for (int j = 0; j < 5; j++)
	{
		if (this->listEquipItems[j].x >= i)
			this->listEquipItems[j].x--;
	}
	this->updateConfig();
}

Vec2 PackConfig::getLevel(int index)
{
	int i = 0;
	for (i = 0; i < this->listSkillLevel.size(); i++)
	{
		if (this->listSkillLevel[i].x == index + 1)
		{
			return this->listSkillLevel[i];
		}
	}
	log("create new item level in pack");
	this->listSkillLevel.push_back(Vec2(index, -1));
	updateConfig();
	return this->listSkillLevel[this->listSkillLevel.size() - 1];
}

void PackConfig::setLevel(int index, int id)
{
	for (int i = 0; i < this->listSkillLevel.size(); i++)
	{
		if (this->listSkillLevel[i].x == index + 1)
		{
			this->listSkillLevel[i].y = id;
			updateConfig();
			break;
		}
	}
}

void PackConfig::addMagicPoint(int number)
{
	this->magicPoint += number;
	updateConfig();
}

void PackConfig::subMagicPoint(int number)
{
	if (this->magicPoint >= number)
	{
		this->magicPoint -= number;
		updateConfig();
	}
}

GameItem* PackConfig::getEquipItemAt(int i)
{
	if (this->listEquipItems.at(i).y == -1 && this->listEquipItems.at(i).z == -1)
		return NULL;
	return ItemsConfig::getInstance()->getItem(this->listEquipItems.at(i).y, this->listEquipItems.at(i).z);
}

void PackConfig::setEquipItemAt(int i, int posInPack, Vec2 item)
{
	this->listEquipItems[i].x = posInPack;
	this->listEquipItems[i].y = item.x;
	this->listEquipItems[i].z = item.y;
	updateConfig();
}

int PackConfig::getTotalAttackPoint()
{
	if (this->listEquipItems[1].x != -1)
	{
		Weapon* weapon = static_cast<Weapon*>(ItemsConfig::getInstance()->getItem(this->listEquipItems[1].y, this->listEquipItems[1].z));
		if (weapon != NULL)
		{
			return weapon->getDamage();
		}
	}
	return 0;
}

int PackConfig::getTotalSprintPoint()
{
	int totalPoint = 0;
	for (int i = 0; i < 5; i++)
	{
		if (i != 1)
		{
			if (this->listEquipItems[i].x != -1)
			{
				EquipItem* item = static_cast<EquipItem*>(ItemsConfig::getInstance()->getItem(this->listEquipItems[i].y, this->listEquipItems[i].z));
				totalPoint += item->getSprint();
			}
		}
	}
	return totalPoint;
}

int PackConfig::getTotalDefensePoint()
{
	int totalPoint = 0;
	for (int i = 0; i < 5; i++)
	{
		if (i != 1)
		{
			if (this->listEquipItems[i].x != -1)
			{
				EquipItem* item = static_cast<EquipItem*>(ItemsConfig::getInstance()->getItem(this->listEquipItems[i].y, this->listEquipItems[i].z));
				totalPoint += item->getDefense();
			}
		}
	}
	return totalPoint;
}

MagicItem* PackConfig::getFirstSkill()
{
	if (this->listEquipItems[5].x != -1)
		return static_cast<MagicItem*>(ItemsConfig::getInstance()->getItem(this->listEquipItems[5].y, this->listEquipItems[5].z));
	return NULL;
}
MagicItem* PackConfig::getSecSkill()
{
	if (this->listEquipItems[6].x != -1)
		return static_cast<MagicItem*>(ItemsConfig::getInstance()->getItem(this->listEquipItems[6].y, this->listEquipItems[6].z));
	return NULL;
}
int PackConfig::getNumberPotionRed()
{
	int number = 0;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == 111 && this->listItems[i].y == 2)
			number++;
	}
	return number;
}
bool PackConfig::usePotionRed()
{
	if (getNumberPotionRed() > 0)
	{
		this->removeItem(Vec2(111, 2));
		return true;
	}
	return false;
}
int PackConfig::getNumberPotionBlue()
{
	int number = 0;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == 111 && this->listItems[i].y == 1)
			number++;
	}
	return number;
}
bool PackConfig::usePotionBlue()
{
	if (getNumberPotionBlue() > 0)
	{
		this->removeItem(Vec2(111, 1));
		return true;
	}
	return false;
}

int PackConfig::removeItemFromCollection(int index, Vec2 item)
{
	int slotPosition = -1;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == item.x && this->listItems[i].y == item.y)
		{
			this->listItems.erase(this->listItems.begin() + i);
			break;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (this->listEquipItems[i].x == index)
		{
			this->listEquipItems[i] = Vec3(-1, -1, -1);
			slotPosition = i;
		}
		else
		{
			if (this->listEquipItems[i].x > index)
			{
				this->listEquipItems[i].x--;
			}
		}
	}
	updateConfig();
	return slotPosition;
}

void PackConfig::upgradeItem(int indexOnPack)
{
	GameItem* item = ItemsConfig::getInstance()->getItem(this->listItems[indexOnPack].x, this->listItems[indexOnPack].y);
	int idType = item->getTypeItem();
	GameItem* nextItem = NULL;
	if (idType == 222)
	{
		Weapon* weapon = static_cast<Weapon*>(item);
		if (weapon->getnextLvlId() != -1)
		{
			nextItem = ItemsConfig::getInstance()->getItem(222, weapon->getnextLvlId());
		}
	}

	if (idType == 333)
	{
		EquipItem* equipItem = static_cast<EquipItem*>(item);
		if (equipItem->getNextLvl() != -1)
		{
			nextItem = ItemsConfig::getInstance()->getItem(333, equipItem->getNextLvl());
		}
	}
	if (nextItem != NULL)
	{
		this->listItems[indexOnPack].y = nextItem->getIdItem();
		for (int i = 0; i < 5; i++)
		{
			if (this->listEquipItems[i].x == indexOnPack)
			{
				this->listEquipItems[i].z = nextItem->getIdItem();
			}
		}
		updateConfig();
	}
}

std::vector<Vec2> PackConfig::getListItemById(int idType)
{
	std::vector<Vec2> sList;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == idType)
		{
			sList.push_back(this->listItems[i]);
		}
	}
	return sList;
}

std::vector<Vec3> PackConfig::getListUpgradeItem()
{
	std::vector<Vec3> sList;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		if (this->listItems[i].x == 222 || this->listItems[i].x == 333)
		{
			sList.push_back(Vec3(i, this->listItems[i].x, this->listItems[i].y));
		}
	}
	return sList;
}

void PackConfig::sortPack()
{
	for (int i = 0; i < this->listItems.size(); i++)
	{
		for (int j = 0; j < this->listItems.size(); j++)
		{
			if (this->listItems[i].x < this->listItems[j].x ||
				(this->listItems[i].x == this->listItems[j].x && this->listItems[i].y < this->listItems[j].y))
			{
				this->swap(i, this->listItems[i], j, this->listItems[j]);
			}
		}
	}
	log("after sort pack");
	for (int i = 0; i < this->listItems.size(); i++)
	{
		log("%d %f %f", i, this->listItems[i].x, this->listItems[i].y);
	}
	log("after sort item equip");
	for (int i = 0; i < this->listEquipItems.size(); i++)
	{
		log("%f %f %f", this->listEquipItems[i].x, this->listEquipItems[i].y, this->listEquipItems[i].z);
	}
	updateConfig();
}

void PackConfig::swap(int inA, Vec2 &a, int inB, Vec2 &b)
{
	//swap 2 element
	int temp = a.x;
	a.x = b.x;
	b.x = temp;

	temp = a.y;
	a.y = b.y;
	b.y = temp;

	//update new index item in list equip item
	for (int i = 0; i < this->listEquipItems.size(); i++)
	{
		if (this->listEquipItems[i].x == inB)
		{
			this->listEquipItems[i].x = inA;
			continue;
		}
		if (this->listEquipItems[i].x == inA)
		{
			this->listEquipItems[i].x = inB;
			continue;
		}
	}
}