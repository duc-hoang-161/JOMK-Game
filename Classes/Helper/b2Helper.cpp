#include "b2Helper.h"

#define PTM_RATIO 32.0

float b2Helper::asValue(float b2Value)
{
	return b2Value * PTM_RATIO;
}

float b2Helper::asb2Value(float value)
{
	return value / PTM_RATIO;
}

b2Vec2 b2Helper::asb2Vec2(cocos2d::Vec2 vec)
{
	b2Vec2 ret;
	ret.x = vec.x / PTM_RATIO;
	ret.y = vec.y / PTM_RATIO;

	return ret;
}

cocos2d::Vec2 b2Helper::asVec2(b2Vec2 vec)
{
	cocos2d::Vec2 ret;
	ret.x = vec.x * PTM_RATIO;
	ret.y = vec.y * PTM_RATIO;

	return ret;
}