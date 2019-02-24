#include "Import\MapPointConfig.h"

MapPointConfig* MapPointConfig::mapPoints = NULL;

MapPointConfig::MapPointConfig()
{

}

MapPointConfig* MapPointConfig::getInstance()
{
	if (mapPoints == NULL)
	{
		mapPoints = new MapPointConfig();
		mapPoints->readConfig();
	}
	return mapPoints;
}

void MapPointConfig::readConfig()
{
	//READ JSON STRING FROM FILE
	auto sharedFileUtils = FileUtils::getInstance();


	static std::string data = sharedFileUtils->getStringFromFile("mapPoint-config.json");
	mapPoints->document.Parse<0>(data.c_str());
	if (mapPoints->document.HasParseError())
	{
		log("LOI PASRE JSON");
	}
	else
	{
		if (document.IsObject() && document.HasMember("mapPoint"))
		{
			rapidjson::Value& value = mapPoints->document["mapPoint"];
			assert(value.IsArray());
			for (int i = 0; i < value.Size(); i++)
			{
				rapidjson::Value& v = value[i];
				assert(v.IsArray());
				Vec2 vec(cocos2d::Value(v[0].GetInt()).asInt(), cocos2d::Value(v[1].GetInt()).asInt());
				points.push_back(vec);
			}
		}
		if (document.IsObject() && document.HasMember("totalPoint"))
		{
			this->totalPoint = document["totalPoint"].GetInt();
		}
	}
}

std::vector<Vec2> MapPointConfig::getMapPointConfig()
{
	return this->points;
}

int MapPointConfig::getTotalMapPoint()
{
	return this->totalPoint;
}