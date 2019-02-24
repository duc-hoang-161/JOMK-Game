#ifndef __TRACE_IMAGE_HELPER_H__
#define __TRACE_IMAGE_HELPER_H__

#include "cocos2d.h"
#include <iostream>
#include <vector>
#include <cocos2d.h>

// Based on PhysicsBodyEditor source ( Aurelien Ribon | http://www.aurelienribon.com/ )
class PolygonHelper {
public:
	static float getPolygonSignedArea(std::vector<cocos2d::Vec2> points);
	static float getPolygonArea(std::vector<cocos2d::Vec2> points);
	static bool isPolygonCGW(std::vector<cocos2d::Vec2> points);
};

// Based on PhysicsBodyEditor source ( Aurelien Ribon | http://www.aurelienribon.com/ )
// Taken from BayazitDecomposer.cs (FarseerPhysics.Common.Decomposition.BayazitDecomposer)
// at http://farseerphysics.codeplex.com
class BayazitDecomposer {
private:
	const static float EPSILON;
	const static int MAX_POLYGON_VERTICES;

	static cocos2d::Vec2 at(int i, std::vector<cocos2d::Vec2> vertices);
	static bool reflex(int i, std::vector<cocos2d::Vec2> vertices);
	static bool right(int i, std::vector<cocos2d::Vec2> vertices);
	static bool left(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);
	static bool leftOn(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);
	static bool right(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);
	static bool rightOn(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);
public:
	static float area(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);
private:
	static float squareDist(cocos2d::Vec2 a, cocos2d::Vec2 b);
	static bool isCounterClockWise(std::vector<cocos2d::Vec2> vect);
	static float getSignedArea(std::vector<cocos2d::Vec2> vect);
	static cocos2d::Vec2 lineIntersect(cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 q1, cocos2d::Vec2 q2);
	static bool floatEquals(float value1, float value2);
	static std::vector<cocos2d::Vec2> copy(int i, int j, std::vector<cocos2d::Vec2> vertices);
	static bool canSee(int i, int j, std::vector<cocos2d::Vec2> vertices);
	static bool lineIntersect(cocos2d::Vec2 point1, cocos2d::Vec2 point2,
		cocos2d::Vec2 point3, cocos2d::Vec2 point4, bool firstIsSegment,
		bool secondIsSegment, cocos2d::Vec2 point);
public:
	static std::vector<std::vector<cocos2d::Vec2>> convexPartition(std::vector<cocos2d::Vec2> verticals);
};

class SimplifyTools {
public:
	static std::vector<cocos2d::Vec2> collinearSimplify(std::vector<cocos2d::Vec2> vertices,
		float collinearityTolerance);

	static bool collinear(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c,
		float tolerance);

	static bool floatInRange(float value, float min, float max);
};

class TraceImageHelper {
private:
	

public:
	static std::vector<std::vector<cocos2d::Vec2>> createPolygon(std::string fileName);
	
};

#endif