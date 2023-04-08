#ifndef __PLAYERHPP__
#define __PLAYERHPP__

#include "BASE.h"
#include "MATH.h"
#include "CAMERA.h"

class __PLAYER {
private:
	vec2f _pos{};
	FLOAT _viewAngle{};

public:
	FLOAT movementSpeed = 1;
	FLOAT rotationSpeed = 1;

	__PLAYER(const vec2f& pos, const FLOAT& viewAngle) 
		: _viewAngle(viewAngle), _pos(pos) { }

	__PLAYER() = default;

	vec2f pos() const {
		return _pos;
	}

	void changePos(vec2f newPos) {
		_pos = newPos;
	}

	void move(vec2f offset) {
		_pos += offset;
	}

	void move(FLOAT dist) {
		_pos += getDir() * dist;
	}
	
	FLOAT viewAngle() const {
		return _viewAngle;
	}

	void rotate(FLOAT angle) {
		_viewAngle += angle;
	}

	void setAngle(FLOAT newAngle) {
		_viewAngle = newAngle;
	}

	vec2f getDir() const {
		return vec2f(cos(_viewAngle), sin(_viewAngle));
	}

	Camera getCam() const {
		// TODO: add realisation
	}
};

#endif