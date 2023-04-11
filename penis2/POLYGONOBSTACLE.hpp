#ifndef __POLYGONOBSTACLEHPP__
#define __POLYGONOBSTACLEHPP__

#include "POLYGONDRAWABLE.hpp"

class PolygonObstacle : public PolygonDrawable {
public:
	using PolygonDrawable::PolygonDrawable;

	bool collide(const PolygonObstacle& obstacle) {
		return Polygonf::intersect(getHitbox(), obstacle.getHitbox());
	}
};

#endif // !__POLYGONOBSTACLEHPP__
