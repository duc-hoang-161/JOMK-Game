#include "Items\GameItem.h"
#include "Items\MagicItem.h"
#include "Items\Weapon.h"
#include "Items\EquipItem.h"
#include "Items\PieceItem.h"
#include "json\rapidjson.h"
#include "json\document.h"
#include "json\stringbuffer.h"
#include "json\writer.h"
#include <fstream>

#ifndef ITEMSCONFIG_H
#define ITEMSCONFIG_H

class ItemsConfig
{
private:
	static ItemsConfig* itemsConfig;
	ItemsConfig();
	std::vector<MagicItem*> listMagic;
	std::vector<Vec2> listDropItem;
	std::vector<Weapon*> listWeapon;
	std::vector<EquipItem*> listEquip;
	std::vector<PieceItem*> listPiece;
	std::vector<std::vector<MagicItem*>> listSkills;
	rapidjson::Document document;
	std::string pathConfigFile = "items_config.json";
public:
	static ItemsConfig* getInstance();
	void readConfig();
	std::string JsonToString(rapidjson::Document &jsonObject);
	GameItem* getItem(int idType, int id);
	std::vector<Vec2> getBuyItem();
	std::vector<Vec2> getBuyItem(int idType);
	std::vector<std::vector<MagicItem*>> getSkillList();
	Vec2 getRandomItem();
};

#endif