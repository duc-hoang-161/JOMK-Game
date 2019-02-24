#ifndef __BOX_2D_HELPER_H__
#define __BOX_2D_HELPER_H__

#include "Box2D\Box2D.h"
#include "cocos2d.h"

class b2Helper {
public:
	static float asb2Value(float value);
	static float asValue(float b2Value);
	static b2Vec2 asb2Vec2(cocos2d::Vec2 vec);
	static cocos2d::Vec2 asVec2(b2Vec2 vec);
};

#endif