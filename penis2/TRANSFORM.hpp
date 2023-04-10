#ifndef __TRANSFORMHPP__
#define __TRANSFORMHPP__

#include "MATH.h"

class Transform {
private:
	vec2f tpos;
	float tangle = 0;
	vec2f tsize = vec2f(1, 1);

public:
	Transform() = default;
	Transform(vec2f tpos, FLOAT tangle, vec2f tsize)
		: tpos(tpos), tangle(tangle), tsize(tsize) { }

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
	void setPos(float x, float y) {
		setPos({ x, y });
	}
	void setAngle(float nAngle) {
		tangle = nAngle;
	}
	void setScale(const vec2f& nScale) {
		tsize = nScale;
	}
	void setScale(float x, float y) {
		setScale({ x, y });
	}
	void setScale(float nScale) {
		tsize = { nScale, nScale };
	}

	void move(const vec2f& vec) {
		tpos = getMat() * vec;
	}
	void move(float x, float y) {
		move({ x, y });
	}
	void rotate(float _angle) {
		tangle += _angle;
	}
	void scale(const vec2f& scale) {
		tsize = tsize * scale;
	}
	void scale(float x, float y) {
		scale({ x, y });
	}
	void scale(float scale) {
		tsize = tsize * scale;
	}
};

void copyTransform(Transform* to, Transform* from) {
	to->setPos(from->getPos());
	to->setAngle(from->getAngle());
	to->setScale(from->getScale());
}

#endif
