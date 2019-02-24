#ifndef __CONVERT_HELPER_H__
#define __CONVERT_HELPER_H__

#include <iostream>
#include <string>
#include <cocos2d.h>
#include <Box2D\Box2D.h>

class ConvertHelper {
public:
	static int asInt(std::string number);	// only number without < 0
};

#endif