#ifndef __POLYGONOBSTACLEHPP__
#define __POLYGONOBSTACLEHPP__

#include "DRAWABLE.hpp"

class PolygonObstacle : public Drawable {
public:
	using Drawable::Drawable;

	bool collide(const PolygonObstacle& obstacle) {
		auto t = getHitbox();
		for (auto& p : t) {
			p += getPos();
		}
		auto t2 = obstacle.getHitbox();
		for (auto& p : t2) {
			p += obstacle.getPos();
		}
		return intersect(t, t2);
	}
};

#endif // !__POLYGONOBSTACLEHPP__
