#ifndef __PLAYERHPP__
#define __PLAYERHPP__

#include "BASE.h"
#include "MATH.h"

class __PLAYER {
private:
	FLOAT _viewAngle{};
	vec2f _pos{};
public:
	vec2f pos() const {
		return _pos;
	}

	void changePos(vec2f newPos) {
		_pos = newPos;
	}

	void move(vec2f) {
		
	}
	
	FLOAT viewAngle() const {
		return _viewAngle;
	}

	void rotate(FLOAT angle) {
		_viewAngle += angle;
	}


};

#endif