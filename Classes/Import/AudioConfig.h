#include "json\rapidjson.h"
#include "json\document.h"
#include "json\stringbuffer.h"
#include "json\writer.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <map>
using namespace cocos2d;
#ifndef AUDIO_CONFIG
#define AUDIO_CONFIG
class AudioConfig
{
private:
	static AudioConfig* audioConfig;
	AudioConfig();
	rapidjson::Document document;
	void readConfig();
	bool isEnable;
	int backgroundVolume;
	int effectVolume;
	std::string rootPath;
	std::string themeFile;
	std::string themeInGame;
	std::string effPath;
	std::string path;
	std::map<std::string, std::string> listEffect;
public:
	static AudioConfig* getInstance();
	bool isAudioEnable();
	int getBackgroundVolume();
	int getEffectVolume();
	std::string getThemeFilePath();
	void updateConfig(bool isEnable, int bgVol, int effVol);
	std::string JsonDocToString(rapidjson::Document &jsonObject);
	std::string JsonValToString(rapidjson::Value &jsonObject);
	void playTheme(int themeNum);
	void playEffect(std::string action);
	void preloadSound();
};

#endif 