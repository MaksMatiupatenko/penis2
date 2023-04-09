#ifndef __TRANSFORMHPP__
#define __TRANSFORMHPP__

#include "MATH.h"

class Transform {
private:
	vec2f tpos;
	float tangle = 0;
	vec2f tsize = vec2f(1, 1);

public:
	mat3f getMat() const {
		return translatem(tpos) * rotatem(tangle) * scalem(tsize);
	}

	vec2f getPos() const {
		return tpos;
	}
	float getAngle() const {
		return tangle;
	}
	vec2f getScale() const {
		return tsize;
	}

	void setPos(const vec2f& nPos) {
		tpos = nPos;
	}
	void setAngle(float nAngle) {
		tangle = nAngle;
	}
	void setScale(const vec2f& nScale) {
		tsize = nScale;
	}

	void move(const vec2f& vec) {
		tpos = tpos + vec;
	}
	void rotate(float _angle) {
		tangle += _angle;
	}
	void scale(const vec2f& scale) {
		tsize = tsize * scale;
	}
};

#endif
