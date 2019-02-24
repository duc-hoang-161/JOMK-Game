#include "Helper\TraceImageHelper.h"

const float BayazitDecomposer::EPSILON = 1.192092896e-07f;
const int BayazitDecomposer::MAX_POLYGON_VERTICES = 8;

float PolygonHelper::getPolygonSignedArea(std::vector<cocos2d::Vec2> points)
{
	if (points.size() < 3)
		return 0;

	float sum = 0;
	for (int i = 0; i < points.size(); i++) {
		cocos2d::Vec2 p1 = points[i];
		cocos2d::Vec2 p2 = (i != points.size() - 1) ? points[i + 1] : points[0];
		sum += (p1.x * p2.y) - (p1.y * p2.x);
	}

	return 0.5 * sum;
}

float PolygonHelper::getPolygonArea(std::vector<cocos2d::Vec2> points)
{
	return std::abs(getPolygonSignedArea(points));
}

bool PolygonHelper::isPolygonCGW(std::vector<cocos2d::Vec2> points)
{
	return getPolygonSignedArea(points) > 0;
}

cocos2d::Vec2 BayazitDecomposer::at(int i, std::vector<cocos2d::Vec2> vertices) {
	int s = vertices.size();
	return vertices[i < 0 ? s - (-i % s) : i % s];
}

bool BayazitDecomposer::reflex(int i, std::vector<cocos2d::Vec2> vertices) {
	return right(i, vertices);
}

bool BayazitDecomposer::right(int i, std::vector<cocos2d::Vec2> vertices) {
	return right(at(i - 1, vertices), at(i, vertices), at(i + 1, vertices));
}

bool BayazitDecomposer::left(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c) {
	return area(a, b, c) > 0;
}

bool BayazitDecomposer::leftOn(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c) {
	return area(a, b, c) >= 0;
}

bool BayazitDecomposer::right(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c) {
	return area(a, b, c) < 0;
}

bool BayazitDecomposer::rightOn(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c) {
	return area(a, b, c) <= 0;
}

float BayazitDecomposer::area(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c) {
	return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

float BayazitDecomposer::squareDist(cocos2d::Vec2 a, cocos2d::Vec2 b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return dx * dx + dy * dy;
}

bool BayazitDecomposer::isCounterClockWise(std::vector<cocos2d::Vec2> vect) {
	// We just return true for lines
	if (vect.size() < 3)
		return true;
	return (getSignedArea(vect) > 0.0f);
}

float BayazitDecomposer::getSignedArea(std::vector<cocos2d::Vec2> vect) {
	int i;
	float area = 0;
	for (i = 0; i < vect.size(); i++) {
		int j = (i + 1) % vect.size();
		area += vect[i].x * vect[j].y;
		area -= vect[i].y * vect[j].x;
	}
	area /= 2.0f;
	return area;
}

cocos2d::Vec2 BayazitDecomposer::lineIntersect(cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 q1, cocos2d::Vec2 q2)
{
	cocos2d::Vec2 i;
	float a1 = p2.y - p1.y;
	float b1 = p1.x - p2.x;
	float c1 = a1 * p1.x + b1 * p1.y;
	float a2 = q2.y - q1.y;
	float b2 = q1.x - q2.x;
	float c2 = a2 * q1.x + b2 * q1.y;
	float det = a1 * b2 - a2 * b1;

	if (!floatEquals(det, 0)) {
		// lines are not parallel
		i.x = (b2 * c1 - b1 * c2) / det;
		i.y = (a1 * c2 - a2 * c1) / det;
	}
	return i;
}

bool BayazitDecomposer::floatEquals(float value1, float value2)
{
	return std::abs(value1 - value2) <= EPSILON;
}

std::vector<cocos2d::Vec2> BayazitDecomposer::copy(int i, int j, std::vector<cocos2d::Vec2> vertices) {
	std::vector<cocos2d::Vec2> p;
	while (j < i)
		j += vertices.size();
	// p.reserve(j - i + 1);
	for (; i <= j; ++i) {
		p.push_back(at(i, vertices));
	}
	return p;
}

bool BayazitDecomposer::canSee(int i, int j, std::vector<cocos2d::Vec2> vertices) {
	if (reflex(i, vertices)) {
		if (leftOn(at(i, vertices), at(i - 1, vertices), at(j, vertices))
			&& rightOn(at(i, vertices), at(i + 1, vertices), at(j, vertices)))
			return false;
	}
	else {
		if (rightOn(at(i, vertices), at(i + 1, vertices), at(j, vertices))
			|| leftOn(at(i, vertices), at(i - 1, vertices), at(j, vertices)))
			return false;
	}
	if (reflex(j, vertices)) {
		if (leftOn(at(j, vertices), at(j - 1, vertices), at(i, vertices))
			&& rightOn(at(j, vertices), at(j + 1, vertices), at(i, vertices)))
			return false;
	}
	else {
		if (rightOn(at(j, vertices), at(j + 1, vertices), at(i, vertices))
			|| leftOn(at(j, vertices), at(j - 1, vertices), at(i, vertices)))
			return false;
	}
	for (int k = 0; k < vertices.size(); ++k) {
		if ((k + 1) % vertices.size() == i || k == i
			|| (k + 1) % vertices.size() == j || k == j) {
			continue; // ignore incident edges
		}
		cocos2d::Vec2 intersectionPoint;
		if (lineIntersect(at(i, vertices), at(j, vertices),
			at(k, vertices), at(k + 1, vertices), true, true,
			intersectionPoint)) {
			return false;
		}
	}
	return true;
}

bool BayazitDecomposer::lineIntersect(cocos2d::Vec2 point1, cocos2d::Vec2 point2,
	cocos2d::Vec2 point3, cocos2d::Vec2 point4, bool firstIsSegment,
	bool secondIsSegment, cocos2d::Vec2 point) {
	point.set(0, 0);
	// these are reused later.
	// each lettered sub-calculation is used twice, except
	// for b and d, which are used 3 times
	float a = point4.y - point3.y;
	float b = point2.x - point1.x;
	float c = point4.x - point3.x;
	float d = point2.y - point1.y;
	// denominator to solution of linear system
	float denom = (a * b) - (c * d);
	// if denominator is 0, then lines are parallel
	if (!(denom >= -EPSILON && denom <= EPSILON)) {
		float e = point1.y - point3.y;
		float f = point1.x - point3.x;
		float oneOverDenom = 1.0f / denom;
		// numerator of first equation
		float ua = (c * e) - (a * f);
		ua *= oneOverDenom;
		// check if intersection point of the two lines is on line segment 1
		if (!firstIsSegment || ua >= 0.0f && ua <= 1.0f) {
			// numerator of second equation
			float ub = (b * e) - (d * f);
			ub *= oneOverDenom;
			// check if intersection point of the two lines is on line
			// segment 2
			// means the line segments intersect, since we know it is on
			// segment 1 as well.
			if (!secondIsSegment || ub >= 0.0f && ub <= 1.0f) {
				// check if they are coincident (no collision in this case)
				if (ua != 0.0f || ub != 0.0f) {
					// There is an intersection
					point.x = point1.x + ua * b;
					point.y = point1.y + ua * d;
					return true;
				}
			}
		}
	}
	return false;
}

std::vector<std::vector<cocos2d::Vec2>> BayazitDecomposer::convexPartition(std::vector<cocos2d::Vec2> vertices)
{
	if (!isCounterClockWise(vertices)) {
		// Reverse
		int left = 0, right = vertices.size() - 1;
		while (left < right) {
			cocos2d::Vec2 tmp = vertices[left];
			vertices[left] = vertices[right];
			vertices[right] = tmp;
		}
	}

	std::vector<std::vector<cocos2d::Vec2>> list;
	float d, lowerDist, upperDist;
	cocos2d::Vec2 p, lowerInt, upperInt;
	int lowerIndex = 0, upperIndex = 0;
	std::vector<cocos2d::Vec2> lowerPoly, upperPoly;

	for (int i = 0; i < vertices.size(); i++) {
		if (reflex(i, vertices)) {
			lowerDist = upperDist = std::numeric_limits<float>::max();
			for (int j = 0; j < vertices.size(); j++) {
				// if line intersects with an edge
				if (left(at(i - 1, vertices), at(i, vertices), at(j, vertices))
					&& rightOn(at(i - 1, vertices), at(i, vertices), at(j - 1, vertices))) {
					// find the point of intersection
					p = lineIntersect(at(i - 1, vertices), at(i, vertices), at(j, vertices), at(j - 1, vertices));
					if (right(at(i + 1, vertices), at(i, vertices), p)) {
						d = squareDist(at(i, vertices), p);
						if (d < lowerDist) {
							// keep only the closest intersection
							lowerDist = d;
							lowerInt = p;
							lowerIndex = j;
						}
					}
				}

				if (left(at(i + 1, vertices), at(i, vertices), at(j + 1, vertices))
						&& rightOn(at(i + 1, vertices), at(i, vertices), at(j, vertices))) {
					p = lineIntersect(at(i + 1, vertices), at(i, vertices), at(j, vertices), at(j + 1, vertices));
					if (left(at(i - 1, vertices), at(i, vertices), p)) {
						d = squareDist(at(i, vertices), p);
						if (d < upperDist) {
							upperDist = d;
							upperIndex = j;
							upperInt = p;
						}
					}
				}
			}
			// if there are no vertices to connect to, choose a point in the
			// middle
			if (lowerIndex == (upperIndex + 1) % vertices.size()) {
				cocos2d::Vec2 sp((lowerInt.x + upperInt.x) / 2,
					(lowerInt.y + upperInt.y) / 2);
				lowerPoly = copy(i, upperIndex, vertices);
				lowerPoly.push_back(sp);
				upperPoly = copy(lowerIndex, i, vertices);
				upperPoly.push_back(sp);
			}
			else {
				double highestScore = 0, bestIndex = lowerIndex;
				while (upperIndex < lowerIndex)
					upperIndex += vertices.size();
				for (int j = lowerIndex; j <= upperIndex; ++j) {
					if (canSee(i, j, vertices)) {
						double score = 1 / (squareDist(at(i, vertices), at(j, vertices)) + 1);
						if (reflex(j, vertices)) {
							if (rightOn(at(j - 1, vertices), at(j, vertices), at(i, vertices))
								&& leftOn(at(j + 1, vertices), at(j, vertices), at(i, vertices))) {
								score += 3;
							}
							else {
								score += 2;
							}
						}
						else {
							score += 1;
						}
						if (score > highestScore) {
							bestIndex = j;
							highestScore = score;
						}
					}
				}
				lowerPoly = copy(i, (int)bestIndex, vertices);
				upperPoly = copy((int)bestIndex, i, vertices);
			}
			std::vector<std::vector<cocos2d::Vec2>> l = convexPartition(lowerPoly);
			for (int i = 0; i < l.size(); i++)
				list.push_back(l[i]);
			std::vector<std::vector<cocos2d::Vec2>> u = convexPartition(upperPoly);
			for (int i = 0; i < u.size(); i++)
				list.push_back(u[i]);
			return list;
		}
	}

	// polygon is already convex
	if (vertices.size() > MAX_POLYGON_VERTICES) {
		lowerPoly = copy(0, vertices.size() / 2, vertices);
		upperPoly = copy(vertices.size() / 2, 0, vertices);
		std::vector<std::vector<cocos2d::Vec2>> l = convexPartition(lowerPoly);
		for (int i = 0; i < l.size(); i++)
			list.push_back(l[i]);
		std::vector<std::vector<cocos2d::Vec2>> u = convexPartition(upperPoly);
		for (int i = 0; i < u.size(); i++)
			list.push_back(u[i]);
	}
	else
		list.push_back(vertices);
	// The polygons are not guaranteed to be with collinear points. We
	// remove
	// them to be sure.
	for (int i = 0; i < list.size(); i++) {
		list[i] = SimplifyTools::collinearSimplify(list[i], 0);
	}
	// Remove empty vertice collections
	for (int i = list.size() - 1; i >= 0; i--) {
		if (list[i].size() == 0)
			list.erase(list.begin() + i);
	}
	return list;
}

std::vector<cocos2d::Vec2> SimplifyTools::collinearSimplify(std::vector<cocos2d::Vec2> vertices,
	float collinearityTolerance) {
	// We can't simplify polygons under 3 vertices
	if (vertices.size() < 3)
		return vertices;
	std::vector<cocos2d::Vec2> simplified;
	for (int i = 0; i < vertices.size(); i++) {
		int prevId = i - 1;
		if (prevId < 0)
			prevId = vertices.size() - 1;
		int nextId = i + 1;
		if (nextId >= vertices.size())
			nextId = 0;
		cocos2d::Vec2 prev = vertices[prevId];
		cocos2d::Vec2 current = vertices[i];
		cocos2d::Vec2 next = vertices[nextId];
		// If they collinear, continue
		if (collinear(prev, current, next, collinearityTolerance))
			continue;
		simplified.push_back(current);
	}
	return simplified;
}

bool SimplifyTools::collinear(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c,
	float tolerance) {
	return floatInRange(BayazitDecomposer::area(a, b, c), -tolerance,
		tolerance);
}

bool SimplifyTools::floatInRange(float value, float min, float max) {
	return (value >= min && value <= max);
}

std::vector<std::vector<cocos2d::Vec2>> 
	TraceImageHelper::createPolygon(std::string fileName)
{
	std::vector<std::vector<cocos2d::Vec2>> polygons;

	return polygons;

	/*std::vector<cocos2d::Vec2> points = cocos2d::AutoPolygon::traceImage(fileName);
	try {
		polygons = BayazitDecomposer::convexPartition(points);
	}
	catch (std::exception e) {
		polygons.clear();
	}*/

	//return polygons;
}