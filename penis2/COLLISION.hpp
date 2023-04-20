#pragma once

#include "MATH.h"

struct Collision {
	bool exists = false;
	vec2f normal{};
	float penDepth{};
};