#ifndef __COLLIDERHPP__
#define __COLLIDERHPP__

#include "TRANSFORM.hpp"
#include "COLLISION.hpp"
#include "GEOMSTRUCTS.hpp"

struct PolygonCollider;

struct Collider {
	Transform* base = nullptr;

	virtual Collision get(const Collider* other) const = 0;

	virtual Collision get(const PolygonCollider* other) const = 0;
};

struct PolygonCollider : public Collider {
	Polygonf poly;

	Polygonf getAbsPoly() const {
		Polygonf poly1 = poly;
		for (auto& p : poly1) {
			p = Collider::base->getMat() * p;
		}
		return poly1;
	}


	PolygonCollider() = default;
	PolygonCollider(const Polygonf& circle1) {
		poly = circle1;
	}

	Collision get(const Collider* other) const override {
		return other->get(this);
	}

	Collision get(const PolygonCollider* other) const override {
		auto poly1 = getAbsPoly();
		auto opoly1 = other->getAbsPoly();

		for (auto& p : opoly1) {
			if (Polygonf::contains(poly1, p)) {
				Collision coll;
				coll.exists = true;
				coll.point = p;
				coll.normal = Polygonf::getNormal(poly1, p);
				
				return coll;
			}
		}
		for (auto& p : poly1) {
			if (Polygonf::contains(opoly1, p)) {
				Collision coll;
				coll.exists = true;
				coll.point = p;
				coll.normal = -Polygonf::getNormal(opoly1, p);

				return coll;
			}
		}

		return Collision{};
	}
};

#endif