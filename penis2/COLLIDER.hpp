#ifndef __COLLIDERHPP__
#define __COLLIDERHPP__

#include "TRANSFORM.hpp"
#include "COLLISION.hpp"
#include "GEOMSTRUCTS.hpp"

struct CircleCollider;
struct PolygonCollider;

struct Collider {
	Transform* base = nullptr;

	virtual Collision get(const Collider* other) const = 0;

	virtual Collision get(const CircleCollider* other) const = 0;

	virtual Collision get(const PolygonCollider* other) const = 0;
};

struct CircleCollider : public Collider {
	Circle circle{};
	
	CircleCollider() = default;
	CircleCollider(Circle circle1) {
		circle = circle1;
	}

	Circle getAbsCircle() const {
		auto c = circle;
		if (base) {
			c.center += base->getPos();
		}
		return c;
	}
	

	Collision get(const Collider* other) const override {
		return other->get(this);
	}

	Collision get(const CircleCollider* other) const override {
		Collision res{};
		//debug << "entered" << std::endl;
		auto c1 = getAbsCircle();
		auto c2 = other->getAbsCircle();
		float r = c1.radius + c2.radius;
		float d = dstsq(c1.center, c2.center);
		//debug << r << " " << d << std::endl;
		//debug << (bool)(base) << (bool)(other->base) << std::endl;
		if (r * r >= d) {
			res.exists = true;
			if (d != 0) {
				d = sqrtf(d);
				vec2f dir(c1.center, c2.center);
				res.normal = normalize(dir);
				res.penDepth = r - d;
			} else {
				res.normal = vec2f(1, 0);
				res.penDepth = c1.radius;
			}
		}
		//debug << "exited" << std::endl;
		return res;
	}

	Collision get(const PolygonCollider* other) const override {
		return { };
	}
};

struct PolygonCollider : public Collider {
	Collision get(const Collider* other) const override {
		return other->get(this);
	}

	Collision get(const CircleCollider* other) const override {
		return Collision{};
	}
};

#endif