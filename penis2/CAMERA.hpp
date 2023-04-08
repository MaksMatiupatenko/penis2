#ifndef __CAMERAHPP__
#define __CAMERAHPP__

#include "TRANSFORM.hpp"

class Camera: public Transform {
private:

public:
	mat3f getMat() const {
		return translatem(-Transform::getPos()) * rotatem(-Transform::getAngle()) * scalem(vec2f(1, 1) / Transform::getScale());
	}
};

#endif
