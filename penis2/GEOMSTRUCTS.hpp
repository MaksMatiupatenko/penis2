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
		return sgn(crss(ap, ray.dir)) == 0 && sgn(dt(ap, ray.dir)) >= 0;
	}
};

struct Segment {
	vec2f a{}, b{};

	Segment() = default;

	Segment(vec2f a, vec2f b) : a(a), b(b) { }
};

struct Circle {
	vec2f center{};
	float radius{};
};

bool intersectg(Circle a, Circle b) {
	float dsts = len(vec2f(a.center, b.center));
	float sumr = a.radius + b.radius;
	return sumr * sumr >= dsts;
}	

bool intersectg(Ray r, Segment s) {
	auto [a, dir] = r;
	auto [b, c] = s;
	vec2f ab(a, b), ac(a, c);
	bool check1 = sgn(crss(dir, ab)) * sgn(crss(dir, ac)) <= 0;
	if (!check1) {
		return false;
	}
	return sgn(dt(dir, ab)) >= 0 && sgn(dt(dir, ac)) >= 0;
}

bool inters(int l1, int r1, int l2, int r2) {
	if (l1 > r1) {
		std::swap(l1, r1);
	}
	if (l2 > r2) {
		std::swap(l2, r2);
	}
	if (l1 > l2) {
		std::swap(l1, l2);
		std::swap(r1, r2);
	}
	return (l2 >= l1 && l2 <= r2);
}

bool intersectg(Segment a, Segment b) {
	if (!inters(a.a.x, a.b.x, b.a.x, b.a.x) ||
		!inters(a.a.y, a.b.y, b.a.y, b.b.y)) {
		return false;
	}
	auto [c, d] = a;
	auto [e, f] = b;
	vec2f cd(c, d);
	vec2f ef(e, f);
	vec2f ce(c, e), cf(c, f);
	vec2f ec(e, c), ed(e, d);
	return sgn(crss(ce, cd)) * sgn(crss(cf, cd)) <= 0 &&
		sgn(crss(ec, ef)) * sgn(crss(ed, ef)) <= 0;
}

bool intersectg(Segment s, Ray r) {
	return intersectg(r, s);
}

#endif // !__GEOMSTRUCTSHPP__
