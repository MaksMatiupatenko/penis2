#ifndef __CAMERAHPP__
#define __CAMERAHPP__

#include "TRANSFORM.hpp"

class Camera: public Transform {
private:
	vec2f viewArea;

public:
	mat3f getMat() const {
		return scalem(vec2f(1, 1) / viewArea) * scalem(vec2f(1, 1) / Transform::getScale()) * rotatem(-Transform::getAngle()) * translatem(-Transform::getPos());
	}

	void setViewArea(const vec2f& area) {
		viewArea = area;
	}
	void setViewArea(float x, float y) {
		viewArea = { x, y };
	}
};

#endif
