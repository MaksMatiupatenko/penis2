#ifndef __GEOMSTRUCTSHPP__
#define __GEOMSTRUCTSHPP__

#include "TRIANGLE.hpp"

struct Ray {
	vec2f a{};
	vec2f dir{};
	
	Ray() = default;

	Ray(vec2f a, vec2f dir) : a(a), dir(dir) { }

	static bool isOn(const Ray& ray, const vec2f& p) {
		vec2f ap(ray.a, p);
		return crss(ap, ray.dir) == 0 && dt(ap, ray.dir) >= 0;
	}
};

struct Segment {
	vec2f a{}, b{};

	Segment() = default;

	Segment(vec2f a, vec2f b) : a(a), b(b) { }
};

bool intersectg(Ray r, Segment s) {
	auto [a, dir] = r;
	auto [b, c] = s;
	vec2f ab(a, b), ac(a, c);
	bool check1 = crss(dir, ab) * crss(dir, ac) <= 0;
	if (!check1) {
		return false;
	}
	return dt(dir, ab) >= 0 && dt(dir, ac) >= 0;
}

bool intersectg(Segment s, Ray r) {
	return intersectg(r, s);
}

#endif // !__GEOMSTRUCTSHPP__
