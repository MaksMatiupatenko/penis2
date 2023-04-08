#ifndef __TRANSFORMHPP__
#define __TRANSFORMHPP__

#include "MATH.h"

class Transform {
private:
	vec2f pos;
	float angle = 0;
	vec2f size = vec2f(1, 1);

public:
	mat3f getMat() const {
		return translatem(pos) * rotatem(angle) * scalem(size);
	}

	vec2f getPos() const {
		return pos;
	}
	float getAngle() const {
		return angle;
	}
	vec2f getScale() const {
		return size;
	}

	void setPos(const vec2f& nPos) {
		pos = nPos;
	}
	void setAngle(float nAngle) {
		angle = nAngle;
	}
	void setScale(const vec2f& nScale) {
		size = nScale;
	}

	void move(const vec2f& vec) {
		pos = pos + vec;
	}
	void rotate(float _angle) {
		angle += _angle;
	}
	void scale(const vec2f& scale) {
		size = size * scale;
	}
};

#endif
