#ifndef __SPRITEHPP__
#define __SPRITEHPP__

#include "TRIANGLEARRAY.hpp"

class Sprite {
private:
	TRARR trarr;

public:
	Sprite() {
		trarr.push(vec2f(0, 0), vec3f(0, 0, 0), vec2f(0, 0));
		trarr.push(vec2f(0, 0), vec3f(0, 0, 0), vec2f(0, 0));
		trarr.push(vec2f(0, 0), vec3f(0, 0, 0), vec2f(0, 0));
	}
};

#endif
