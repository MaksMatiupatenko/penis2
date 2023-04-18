#ifndef __POLYGONOBSTACLEHPP__
#define __POLYGONOBSTACLEHPP__

#include "POLYGONDRAWABLE.hpp"

class PolygonObstacle : public PolygonDrawable {
public:
	using PolygonDrawable::PolygonDrawable;

	bool collide(const PolygonObstacle& obstacle) {
		auto t = getHitbox();
		for (auto& p : t) {
			p += getPos();
		}
		auto t2 = obstacle.getHitbox();
		for (auto& p : t2) {
			p += obstacle.getPos();
		}
		return Polygonf::intersect(t, t2);
	}

    vec2f getCollNormal(const PolygonObstacle& obstacle) {
		auto t = getHitbox();
		for (auto& p : t) {
			p += getPos();
		}
		auto t2 = obstacle.getHitbox();
		for (auto& p : t2) {
			p += obstacle.getPos();
		}
		return Polygonf::getCollNormal(t, t2);
	}
};

#endif // !__POLYGONOBSTACLEHPP__