#ifndef __PLAYERHPP__
#define __PLAYERHPP__

#include "BASE.h"
#include "MATH.h"
#include "CAMERA.h"
#include "POLYGONOBSTACLE.hpp"

class __PLAYER : public Transform {
private:
	PolygonObstacle* obstacle;

	void initTrarr() {
		const FLOAT radius = 0.03f;
		vec2f cur = { radius, 0 };
		const int N = 4;
		const FLOAT rot = 2 * PI / N;
		Polygonf polygon;
		for (int n = 0; n < N; ++n, cur = rotatem(rot) * cur) {
			polygon.push(cur);
		}
		obstacle = new PolygonObstacle(polygon, GLRED);
	}
	
public:
	FLOAT movementSpeed = 1;
	FLOAT rotationSpeed = 1;

	__PLAYER(const vec2f& pos, const FLOAT& viewAngle) 
		: Transform(pos, viewAngle, vec2f(1, 1)) 
	{
		initTrarr();
	}

	__PLAYER() {
		initTrarr();
	}

	Camera getCam() const {
		// TODO: add realisation
		throw emptyRealisation;
	}

	void draw(const Camera& camera) {
		copyTransform(obstacle, this);
		obstacle->draw(camera);
	}

	bool collide(const PolygonObstacle& obstacle1) {
		copyTransform(obstacle, this);
		return obstacle->collide(obstacle1);
	}

	~__PLAYER() {
		delete obstacle;
	}
};

#endif