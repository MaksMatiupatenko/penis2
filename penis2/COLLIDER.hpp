#ifndef __COLLIDERHPP__
#define __COLLIDERHPP__

#include "TRANSFORM.hpp"
#include "COLLISION.hpp"

struct CircleCollider;
struct PolygonCollider;

struct Collider {
protected:
	Transform* base = nullptr;

public:

	virtual Collision get(const Collider* other) const = 0;

	virtual Collision get(const CircleCollider* other) const = 0;

	virtual Collision get(const PolygonCollider* other) const = 0;
};

struct CircleCollider : public Collider {
	Collision get(const Collider* other) const override {
		return other->get(this);
	}

	Collision get(const CircleCollider* other) const override {
		return Collision{};
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