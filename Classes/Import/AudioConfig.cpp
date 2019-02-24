#include "Import\AudioConfig.h"
#include <fstream>
AudioConfig* AudioConfig::audioConfig = NULL;

AudioConfig::AudioConfig()
{
	isEnable = true;
	backgroundVolume = 100;
	effectVolume = 100;
}

AudioConfig* AudioConfig::getInstance()
{
	if (audioConfig == NULL)
	{
		audioConfig = new AudioConfig();
		audioConfig->path = FileUtils::getInstance()->getWritablePath() + "audio-config.json";
		audioConfig->readConfig();
		audioConfig->preloadSound();
	}
	return audioConfig;
}

void AudioConfig::readConfig()
{
	//FILE OBJECT TO READ JSON STRING FROM FILE
	auto sharedFileUtils = FileUtils::getInstance();
	//get local config
	static std::string data = sharedFileUtils->getStringFromFile(path);
	//get def config
	std::string defData = sharedFileUtils->getStringFromFile("audio-config.json");
	rapidjson::Document defDocument;
	defDocument.Parse<0>(defData.c_str());
	//read def json config audio
	if (defDocument.HasParseError())
	{
		log("ERROR PASRE JSON");
	}
	else
	{
		if (defDocument.IsObject() && defDocument.HasMember("sound"))
		{
			rapidjson::Value& value = defDocument["sound"];
			assert(value.IsObject());
			if (value.HasMember("rootPath"))
			{
				rapidjson::Value& v = value["rootPath"];
				audioConfig->rootPath = cocos2d::Value(v.GetString()).asString();
			}
			if (value.HasMember("theme"))
			{
				rapidjson::Value& v = value["theme"];
				audioConfig->themeFile = cocos2d::Value(v.GetString()).asString();
			}
			this->themeInGame = cocos2d::Value(value["themeInGame"].GetString()).asString();
			this->effPath = cocos2d::Value(value["effectRootPath"].GetString()).asString();

			rapidjson::Value& valueEff = value["effect"];
			assert(valueEff.IsArray());
			if (valueEff.IsArray())
			{
				for (int i = 0; i < valueEff.Size(); i++)
				{
					rapidjson::Value& eff = valueEff[i];
					assert(eff.IsObject());
					this->listEffect.insert(std::pair<std::string, std::string>(cocos2d::Value(eff["action"].GetString()).asString(),
						cocos2d::Value(eff["src"].GetString()).asString()));
				}
			}
		}
	}
	//if local config not save yet
	if (data.length() <= 5)
	{
		//save default config to local
		std::fstream f;
		f.open(path.c_str(), std::ios::out);
		if (f.fail())
		{
			log("can not create file %s", path.c_str());
			return;
		}
		else
		{
			FileUtils::getInstance()->writeStringToFile(JsonValToString(defDocument["volume"]), path);
			log("done write first audio config");
			data = sharedFileUtils->getStringFromFile(path);
		}
	}
	//CONVERT STRING LOCAL DATA TO JSON FORMAT
	audioConfig->document.Parse<0>(data.c_str());
	if (audioConfig->document.HasParseError())
	{
		log("ERROR PASRE JSON");
	}
	else
	{
		//read local config
		if (audioConfig->document.IsObject())
		{
			assert(audioConfig->document.IsObject());
			if (audioConfig->document.HasMember("isEnableAudio"))
			{
				rapidjson::Value& v = audioConfig->document["isEnableAudio"];
				audioConfig->isEnable = cocos2d::Value(v.GetBool()).asBool();
			}
			if (audioConfig->document.HasMember("backgroundVolume"))
			{
				rapidjson::Value& v = audioConfig->document["backgroundVolume"];
				audioConfig->backgroundVolume = cocos2d::Value(v.GetInt()).asInt();
			}
			if (audioConfig->document.HasMember("effectVolume"))
			{
				rapidjson::Value& v = audioConfig->document["effectVolume"];
				audioConfig->effectVolume = cocos2d::Value(v.GetInt()).asInt();
			}
		}
	}
	
	
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(this->backgroundVolume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(this->effectVolume);
	log("audio %d, volume %d, eff %d", isEnable ? 1 : 0, this->backgroundVolume, this->effectVolume);
	
}

bool AudioConfig::isAudioEnable()
{
	log("audio %d, volume %d, eff %d", isEnable ? 1 : 0, this->backgroundVolume, this->effectVolume);
	return isEnable;
}

int AudioConfig::getBackgroundVolume()
{
	log("volume %d", this->backgroundVolume);
	return this->backgroundVolume;
}

int AudioConfig::getEffectVolume()
{
	log("eff %d", this->effectVolume);
	return this->effectVolume;
}

std::string AudioConfig::JsonDocToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}

std::string AudioConfig::JsonValToString(rapidjson::Value &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}

void AudioConfig::updateConfig(bool isEnable, int bgVol, int effVol)
{
	if (this->isEnable != isEnable || backgroundVolume != bgVol || effectVolume != effVol)
	{
		this->isEnable = isEnable;
		this->backgroundVolume = bgVol;
		this->effectVolume = effVol;
		if (isEnable)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgVol/100.0f);
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effVol / 100.0f);
			log("turn on audio");
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
			log("turn off audio");
		}
		if (document.IsObject())
		{
			assert(document.IsObject());
			if (document.HasMember("isEnableAudio"))
			{
				rapidjson::Value& v = document["isEnableAudio"];
				v.SetBool(isEnable);
			}
			if (document.HasMember("backgroundVolume"))
			{
				rapidjson::Value& v = document["backgroundVolume"];
				v.SetInt(bgVol);
			}
			if (document.HasMember("effectVolume"))
			{
				rapidjson::Value& v = document["effectVolume"];
				v.SetInt(effVol);
			}
			FileUtils::getInstance()->writeStringToFile(JsonDocToString(document), path);
			log("done write new audio config");
		}
	}
}

std::string AudioConfig::getThemeFilePath()
{
	return this->rootPath + this->themeFile;
}
void AudioConfig::playTheme(int themeNum)
{
	if (isEnable)
	{
		switch (themeNum)
		{
			case 1:
			{
				std::string path = this->rootPath + this->themeInGame;
				CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(path.c_str(), true);
				log("play theme in game");
				break;
			}
			default:
			{
				std::string path = this->rootPath + this->themeFile;
				CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(path.c_str(), true);
				log("play theme");
				break;
			}
		}
	}
	else
	{
		log("can not play theme when disable audio");
	}
}

void AudioConfig::playEffect(std::string action)
{
	if (isEnable)
	{
		std::string path = this->rootPath + this->effPath + this->listEffect[action];
		unsigned int k = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(path.c_str(), false);
		log("play %s", path.c_str());
	}
	else
	{
		log("can not play %s effect when disable audio", action.c_str());
	}
}

void AudioConfig::preloadSound()
{
	//preload theme
	std::string pathTheme = this->rootPath + this->themeFile;
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pathTheme.c_str());
	log("preload %s", pathTheme.c_str());
	std::string pathThemeInGame = this->rootPath + this->themeInGame;
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pathThemeInGame.c_str());
	log("preload %s", pathThemeInGame.c_str());
	//preload effect
	for (std::map<std::string, std::string>::iterator eff = this->listEffect.begin(); eff != this->listEffect.end(); eff++)
	{
		std::string pathEff = this->rootPath + this->effPath + eff->second;
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pathEff.c_str());
		log("preload %s", pathEff.c_str());
	}
}