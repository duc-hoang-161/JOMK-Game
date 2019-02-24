#include "ItemsConfig.h"

ItemsConfig* ItemsConfig::itemsConfig = NULL;
ItemsConfig::ItemsConfig()
{

}
ItemsConfig* ItemsConfig::getInstance()
{
	if (itemsConfig == NULL)
	{
		itemsConfig = new ItemsConfig();
		itemsConfig->readConfig();
	}
	return itemsConfig;
}


std::string ItemsConfig::JsonToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}

void ItemsConfig::readConfig()
{
	auto sharedFileUtils = FileUtils::getInstance();
	static std::string data = sharedFileUtils->getStringFromFile(pathConfigFile);
	log("start read items config");
	itemsConfig->document.Parse<0>(data.c_str());
	if (itemsConfig->document.HasParseError())
	{
		log("ERROR PASRE JSON");
	}
	else
	{
		//READ MAGIC CONFIG
		if (document.IsObject() && document.HasMember("magic"))
		{
				
			rapidjson::Value& value = itemsConfig->document["magic"];
			if (value.HasMember("idType") && value.HasMember("items"))
			{
				int idType = cocos2d::Value(value["idType"].GetInt()).asInt();
				log("11111 %d", idType);
				rapidjson::Value& valueItem = value["items"];
				assert(valueItem.IsArray());
				for (int i = 0; i < valueItem.Size(); i++)
				{
					rapidjson::Value&  v = valueItem[i];
					assert(v.IsObject());
					MagicItem* magicItem = new MagicItem();
					magicItem->setTypeItem(idType);
					if (v.HasMember("id") && v.HasMember("nameItem") && v.HasMember("info") && v.HasMember("price") && v.HasMember("isDropItem") && v.HasMember("srcImage"))
					{
						magicItem->setIdItem(cocos2d::Value(v["id"].GetInt()).asInt());
						magicItem->setNameItem(cocos2d::Value(v["nameItem"].GetString()).asString());
						magicItem->setInfoItem(cocos2d::Value(v["info"].GetString()).asString());
						magicItem->setPrice(cocos2d::Value(v["price"].GetInt()).asInt()>0 ? cocos2d::Value(v["price"].GetInt()).asInt() / 10 : cocos2d::Value(v["price"].GetInt()).asInt());
						magicItem->setDropItem(cocos2d::Value(v["isDropItem"].GetBool()).asBool());
						magicItem->setSrcImage(cocos2d::Value(v["srcImage"].GetString()).asString());
						magicItem->setSrcDropImage(cocos2d::Value(v["srcImageInGame"].GetString()).asString());
						magicItem->setManaToUse(cocos2d::Value(v["manaToUse"].GetInt()).asInt());
						magicItem->setDamage(cocos2d::Value(v["damage"].GetInt()).asInt());
						magicItem->setSkill(cocos2d::Value(v["isSkill"].GetBool()).asBool());
					}
					log("%d %d", magicItem->getIdItem(), magicItem->getTypeItem());
					listMagic.push_back(magicItem);
				}
			}
			if (value.HasMember("skill"))
			{
				rapidjson::Value& valueSkillList = value["skill"];
				assert(valueSkillList.IsArray());
				for (int i = 0; i < valueSkillList.Size(); i++)
				{
					rapidjson::Value& valueSkill = valueSkillList[i];
					assert(valueSkillList.IsArray());
					std::vector<MagicItem*> magicSkill;
					for (int j = 0; j < valueSkill.Size(); j++)
					{
						rapidjson::Value& skillLevel = valueSkill[j];
						MagicItem* skillItem = new MagicItem();
						skillItem->setTypeItem(111);
						if (skillLevel.HasMember("id") && skillLevel.HasMember("nameItem") && skillLevel.HasMember("info") && skillLevel.HasMember("price") && skillLevel.HasMember("isDropItem") && skillLevel.HasMember("srcImage"))
						{
							skillItem->setIdItem(cocos2d::Value(skillLevel["id"].GetInt()).asInt());
							skillItem->setNameItem(cocos2d::Value(skillLevel["nameItem"].GetString()).asString());
							skillItem->setInfoItem(cocos2d::Value(skillLevel["info"].GetString()).asString());
							skillItem->setPrice(cocos2d::Value(skillLevel["price"].GetInt()).asInt());
							skillItem->setDropItem(cocos2d::Value(skillLevel["isDropItem"].GetBool()).asBool());
							skillItem->setSrcImage(cocos2d::Value(skillLevel["srcImage"].GetString()).asString());
							skillItem->setSrcDropImage(cocos2d::Value(skillLevel["srcImageInGame"].GetString()).asString());
							skillItem->setManaToUse(cocos2d::Value(skillLevel["manaToUse"].GetInt()).asInt());
							skillItem->setDamage(cocos2d::Value(skillLevel["damage"].GetInt()).asInt());
							skillItem->setSkill(cocos2d::Value(skillLevel["isSkill"].GetBool()).asBool());
						}
						magicSkill.push_back(skillItem);
						log("%s", skillItem->getSrc().c_str());
					}
					log("===%s", magicSkill.at(0)->getNameItem().c_str());
					listSkills.push_back(magicSkill);
				}
			}
		}
		//READ WEAPON CONFIG
		if (document.IsObject() && document.HasMember("weapon"))
		{
			rapidjson::Value& value = itemsConfig->document["weapon"];
			if (value.HasMember("idType") && value.HasMember("items"))
			{
				int idType = cocos2d::Value(value["idType"].GetInt()).asInt();
				log("222 %d", idType);
				rapidjson::Value& valueItem = value["items"];
				assert(valueItem.IsArray());
				for (int i = 0; i < valueItem.Size(); i++)
				{
					rapidjson::Value& valueWeapon = valueItem[i];
					Weapon* weapon = new Weapon();
					weapon->setTypeItem(idType);
					weapon->setIdItem(cocos2d::Value(valueWeapon["id"].GetInt()).asInt());
					weapon->setNameItem(cocos2d::Value(valueWeapon["nameItem"].GetString()).asString());
					weapon->setInfoItem(cocos2d::Value(valueWeapon["info"].GetString()).asString());
					weapon->setPrice(cocos2d::Value(valueWeapon["price"].GetInt()).asInt()/10);
					weapon->setDropItem(cocos2d::Value(valueWeapon["isDropItem"].GetBool()).asBool());
					weapon->setSrcImage(cocos2d::Value(valueWeapon["srcImage"].GetString()).asString());
					weapon->setSrcDropImage(cocos2d::Value(valueWeapon["srcImageInGame"].GetString()).asString());
					weapon->setWeaponLvl(cocos2d::Value(valueWeapon["currentLvl"].GetInt()).asInt());
					weapon->setnextLvlId(cocos2d::Value(valueWeapon["nextLvlId"].GetInt()).asInt());
					weapon->setDamage(cocos2d::Value(valueWeapon["damage"].GetInt()).asInt());
					log("%d", weapon->getDamage());
					listWeapon.push_back(weapon);
				}
			}
		}
		//READ EQUIP CONFIG
		if (document.IsObject() && document.HasMember("equipItems"))
		{
			rapidjson::Value& value = itemsConfig->document["equipItems"];
			if (value.HasMember("idType") && value.HasMember("items"))
			{
				int idType = cocos2d::Value(value["idType"].GetInt()).asInt();
				log("222 %d", idType);
				rapidjson::Value& valueItem = value["items"];
				assert(valueItem.IsArray());
				for (int i = 0; i < valueItem.Size(); i++)
				{
					rapidjson::Value& valueEquip = valueItem[i];
					EquipItem* equipItem = new EquipItem();
					equipItem->setTypeItem(idType);
					equipItem->setIdItem(cocos2d::Value(valueEquip["id"].GetInt()).asInt());
					equipItem->setNameItem(cocos2d::Value(valueEquip["nameItem"].GetString()).asString());
					equipItem->setInfoItem(cocos2d::Value(valueEquip["info"].GetString()).asString());
					equipItem->setPrice(cocos2d::Value(valueEquip["price"].GetInt()).asInt()/10);
					equipItem->setDropItem(cocos2d::Value(valueEquip["isDropItem"].GetBool()).asBool());
					equipItem->setSrcImage(cocos2d::Value(valueEquip["srcImage"].GetString()).asString());
					equipItem->setSrcDropImage(cocos2d::Value(valueEquip["srcImageInGame"].GetString()).asString());
					equipItem->setCurrentLvl(cocos2d::Value(valueEquip["currentLvl"].GetInt()).asInt());
					equipItem->setNextLvl(cocos2d::Value(valueEquip["nextLvlId"].GetInt()).asInt());
					equipItem->setDefense(cocos2d::Value(valueEquip["defense"].GetInt()).asInt());
					equipItem->setSprint(cocos2d::Value(valueEquip["sprint"].GetInt()).asInt());
					equipItem->setPositionEquip(cocos2d::Value(valueEquip["position"].GetInt()).asInt());
					log("%s", equipItem->getNameItem().c_str());
					log("%d", equipItem->getPrice());
					listEquip.push_back(equipItem);
				}
			}
		}
		//READ PIECE CONFIG
		if (document.IsObject() && document.HasMember("pieceItem"))
		{
			rapidjson::Value& value = itemsConfig->document["pieceItem"];
			if (value.HasMember("idType") && value.HasMember("items"))
			{
				int idType = cocos2d::Value(value["idType"].GetInt()).asInt();
				log("444 %d", idType);
				rapidjson::Value& valueItem = value["items"];
				assert(valueItem.IsArray());
				for (int i = 0; i < valueItem.Size(); i++)
				{
					rapidjson::Value& valuePiece = valueItem[i];
					PieceItem* pieceItem = new PieceItem();
					pieceItem->setTypeItem(idType);
					pieceItem->setIdItem(cocos2d::Value(valuePiece["id"].GetInt()).asInt());
					pieceItem->setNameItem(cocos2d::Value(valuePiece["nameItem"].GetString()).asString());
					pieceItem->setInfoItem(cocos2d::Value(valuePiece["info"].GetString()).asString());
					pieceItem->setPrice(cocos2d::Value(valuePiece["price"].GetInt()).asInt()/10);
					pieceItem->setDropItem(cocos2d::Value(valuePiece["isDropItem"].GetBool()).asBool());
					pieceItem->setSrcImage(cocos2d::Value(valuePiece["srcImage"].GetString()).asString());
					pieceItem->setSrcDropImage(cocos2d::Value(valuePiece["srcImageInGame"].GetString()).asString());
					pieceItem->setKey(cocos2d::Value(valuePiece["key"].GetInt()).asInt());
					pieceItem->setValue(cocos2d::Value(valuePiece["value"].GetInt()).asInt());
					log("%d %d %d", pieceItem->getTypeItem(), pieceItem->getKey(), pieceItem->getIdItem());
					listPiece.push_back(pieceItem);
				}
			}
		}
		//if (this->listDropItem.size() == 0)
		//{
		for (int i = 0; i < this->listWeapon.size(); i++)
		{
			if (this->listWeapon[i]->isDropItem())
			{
				listDropItem.push_back(Vec2(this->listWeapon[i]->getTypeItem(), this->listWeapon[i]->getIdItem()));
			}
		}
		for (int i = 0; i < this->listEquip.size(); i++)
		{
			if (this->listEquip[i]->isDropItem())
			{
				listDropItem.push_back(Vec2(this->listEquip[i]->getTypeItem(), this->listEquip[i]->getIdItem()));
			}
		}
		for (int i = 0; i < this->listPiece.size(); i++)
		{
			if (this->listPiece[i]->isDropItem())
			{
				listDropItem.push_back(Vec2(this->listPiece[i]->getTypeItem(), this->listPiece[i]->getIdItem()));
			}
		}
		//}
		log("read done");
	}
}


GameItem* ItemsConfig::getItem(int idType, int id)
{
	switch (idType)
	{
		case 111:
		{
			for (int i = 0; i < listMagic.size(); i++)
			{
				if (listMagic[i]->getIdItem() == id)
					return listMagic[i];
			}
			for (int i = 0; i < listSkills.size(); i++)
			{
				for (int j = 0; j < listSkills[i].size(); j++)
				{
					if (listSkills[i][j]->getIdItem() == id)
						return listSkills[i][j];
				}
			}
			return NULL;
			break;
		}
		case 222:
		{
			for (int i = 0; i < listWeapon.size(); i++)
			{
				if (listWeapon[i]->getIdItem() == id)
					return listWeapon[i];
			}
			return NULL;
			break;
		}
		case 333:
		{
			for (int i = 0; i < listEquip.size(); i++)
			{
				if (listEquip[i]->getIdItem() == id)
				{

					return listEquip[i];
				}

					
			}
			return NULL;
			break;
		}
		case 444:
		{
			for (int i = 0; i < listPiece.size(); i++)
			{
				if (listPiece[i]->getIdItem() == id)
				{

					return listPiece[i];
				}


			}
			return NULL;
			break;
		}
		default:
		{

				   return NULL;
				   break;
		}
	}
}

std::vector<Vec2> ItemsConfig::getBuyItem()
{
	std::vector<Vec2> listItemBuy;
	for (int i = 0; i < listMagic.size(); i++)
	{
		if (listMagic[i]->getPrice() > 0)
			listItemBuy.push_back(Vec2(listMagic[i]->getTypeItem(), listMagic[i]->getIdItem()));
	}
	for (int i = 0; i < listWeapon.size(); i++)
	{
		if (listWeapon[i]->getPrice() > 0)
			listItemBuy.push_back(Vec2(listWeapon[i]->getTypeItem(), listWeapon[i]->getIdItem()));
	}
	for (int i = 0; i < listEquip.size(); i++)
	{
		if (listEquip[i]->getPrice() > 0)
			listItemBuy.push_back(Vec2(listEquip[i]->getTypeItem(), listEquip[i]->getIdItem()));
	}
	for (int i = 0; i < listPiece.size(); i++)
	{
		if (listPiece[i]->getPrice() > 0)
			listItemBuy.push_back(Vec2(listPiece[i]->getTypeItem(), listPiece[i]->getIdItem()));
	}
	return listItemBuy;
}

std::vector<Vec2> ItemsConfig::getBuyItem(int idType)
{
	std::vector<Vec2> listItemBuy;
	switch (idType)
	{
		case 222:
		{
			for (int i = 0; i < listWeapon.size(); i++)
			{
				if (listWeapon[i]->getPrice() > 0)
					listItemBuy.push_back(Vec2(listWeapon[i]->getTypeItem(), listWeapon[i]->getIdItem()));
			}
			break;
		}
		case 333:
		{
			for (int i = 0; i < listEquip.size(); i++)
			{
				if (listEquip[i]->getPrice() > 0)
					listItemBuy.push_back(Vec2(listEquip[i]->getTypeItem(), listEquip[i]->getIdItem()));
			}
			break;
		}
		default:
		{
			for (int i = 0; i < listMagic.size(); i++)
			{
				if (listMagic[i]->getPrice() > 0)
					listItemBuy.push_back(Vec2(listMagic[i]->getTypeItem(), listMagic[i]->getIdItem()));
			}
			for (int i = 0; i < listPiece.size(); i++)
			{
				if (listPiece[i]->getPrice() > 0)
					listItemBuy.push_back(Vec2(listPiece[i]->getTypeItem(), listPiece[i]->getIdItem()));
			}
			break;
		}
	}
	return listItemBuy;
}

std::vector<std::vector<MagicItem*>> ItemsConfig::getSkillList()
{
	
	return this->listSkills;
}
Vec2 ItemsConfig::getRandomItem()
{
	
	if (this->listDropItem.size() != 0)
	{
		int randIndex = RandomHelper::random_int(0, (int)this->listDropItem.size()-1);
		return this->listDropItem[randIndex];
	}
	return Vec2(444, 1);
}
