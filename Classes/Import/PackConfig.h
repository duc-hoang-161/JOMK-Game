#include "cocos2d.h"
#include "Items\GameItem.h"
#include "json\rapidjson.h"
#include "json\document.h"
#include "json\stringbuffer.h"
#include "json\writer.h"
#include <fstream>
#include "Import\ItemsConfig.h"

using namespace cocos2d;

#ifndef PACK_CONFIG_H
#define PACK_CONFIG_H

class PackConfig
{
private:
	static PackConfig* packConfig;
	PackConfig();
	std::string pathConfigFile = "pack_config.json";
	rapidjson::Document document;
	std::string JsonToString(rapidjson::Document &jsonObject);
	int MAX_SLOT;
	int expandSlotStep;
	int priceUpgrade;
	int availableSlot;
	int coins;
	int magicPoint;
	int passedPoint;
	std::vector<Vec2> listItems;
	std::vector<Vec2> listSkillLevel;
	std::vector<Vec3> listEquipItems;
public:
	static PackConfig* getInstance();
	void readConfig();
	void updateConfig();
	void addItem(Vec2 newItem);
	void addItem(int idType, int idItem);
	int getTotalSlot();
	int getAvailableSlot();
	int getPriceUpgrade();
	int getExpandStep();
	int getCoins();
	int getMagicPoint();
	int getPassedPoint();
	bool passNewPoint(int level);
	GameItem* getEquipItemAt(int i);
	void setEquipItemAt(int i, int posInPack, Vec2 item);
	void addCoins(int number);
	void subCoins(int number);
	void addMagicPoint(int number);
	void subMagicPoint(int number);
	void expandPackSize();
	void removeItem(Vec2 item);
	int removeItemFromCollection(int index, Vec2 item);
	std::vector<Vec2> getItemsPack();
	//std::vector<Vec2> getSkillLevel();
	Vec2 getLevel(int index);
	void setLevel(int index, int id);
	int getTotalAttackPoint();
	int getTotalSprintPoint();
	int getTotalDefensePoint();
	MagicItem* getFirstSkill();
	MagicItem* getSecSkill();
	int getNumberPotionRed();
	bool usePotionRed();
	int getNumberPotionBlue();
	bool usePotionBlue();
	void upgradeItem(int indexOnPack);
	std::vector<Vec2> getListItemById(int idType);
	std::vector<Vec3> getListUpgradeItem();
	void sortPack();
	void swap(int inA, Vec2 &a, int inB, Vec2 &b);
};

#endif