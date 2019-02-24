#include "json\rapidjson.h"
#include "json\document.h"
#include "json\stringbuffer.h"
#include "json\writer.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

#ifndef MAPPOINTCONFIG_H
#define MAPPOINTCONFIG_H
class MapPointConfig
{
private:
	static MapPointConfig* mapPoints;
	MapPointConfig();
	rapidjson::Document document;
	std::vector<Vec2> points;
	int totalPoint;
	void readConfig();
public:
	static MapPointConfig* getInstance();
	std::vector<Vec2> getMapPointConfig();
	int getTotalMapPoint();
};

#endif 