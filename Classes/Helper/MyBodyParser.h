#pragma once
#ifndef _MY_BODY_PARSER_H_
#define _MY_BODY_PARSER_H_
#include <string>
#include <vector>
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#include "json/document.h"
#include "json\writer.h"
#include "json\stringbuffer.h"
#include "json\ostreamwrapper.h"
#include "Helper\TraceImageHelper.h"

#include "Box2D\Box2D.h"

class FixtureDef {
public:
	FixtureDef()
		: next(NULL) {}

	~FixtureDef() {
		delete next;
		delete fixture.shape;
	}

	FixtureDef *next;
	b2FixtureDef fixture;
	int callbackData;
};

class MyBodyParser {
private:
	MyBodyParser(){}
	rapidjson::Document doc;
	rapidjson::Document config;

	bool parseDoc(unsigned char* buffer, long length);
	bool parseConfig(unsigned char *buffer, long length);

	cocos2d::Vec2 anchor;
public:
	static MyBodyParser* getInstance();
	bool parseJsonFile(const std::string& pFile, const std::string &config);
	void clearCache();
	bool containKey(const std::string &key);

		// Return 2 physics body
		// Normal and flippedX
	std::pair<FixtureDef*, FixtureDef*>
		bodyFormJson(const std::string& name);
};

#endif //_MY_BODY_PARSER_H_