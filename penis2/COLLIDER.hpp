#ifndef __COLLIDERHPP__
#define __COLLIDERHPP__

#include "TRANSFORM.hpp"
#include "COLLISION.hpp"
#include "GEOMSTRUCTS.hpp"

struct PolygonCollider;

class Collider {
public:
	Transform* base = nullptr;

	virtual Collision get(const Collider* other) const = 0;

	virtual Collision get(const PolygonCollider* other) const = 0;
};

class PolygonCollider : public Collider {
private:
	float rad;
	vec2f center;

public:
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
		poly.normalizeOrder();

		center = getCenter(poly);
		for (auto p : poly) {
			rad = max(rad, lenght(p - center));
		}
	}

	Collision get(const Collider* other) const override {
		return other->get(this);
	}

	Collision get(const PolygonCollider* other) const override {
		if (lenght(*base * center - *other->base * other->center) > rad + other->rad) {
			return Collision{};
		}

		auto poly1 = getAbsPoly();
		auto opoly1 = other->getAbsPoly();

		std::vector <vec2f> dirs;
		for (int i = 0; i < poly1.size(); ++i) {
			dirs.push_back(poly1.get(i + 1) - poly1.get(i));
		}
		for (int i = 0; i < opoly1.size(); ++i) {
			dirs.push_back(opoly1.get(i + 1) - opoly1.get(i));
		}

		float aln = 0;
		vec2f norm = { 0, 0 };
		for (auto dir : dirs) {
			dir = normalize(dir);
			float p11, p12, p21, p22;
			{
				auto [i1, i2] = getParTangents(dir, poly1);
				p11 = cross(dir, poly1[i1]);
				p12 = cross(dir, poly1[i2]);
			}
			{
				auto [i1, i2] = getParTangents(dir, opoly1);
				p21 = cross(dir, opoly1[i1]);
				p22 = cross(dir, opoly1[i2]);
			}

			if (p11 > p22 || p21 > p12) return Collision{};
			
			if (norm == vec2f(0, 0) || p22 - p11 < aln) {
				aln = p22 - p11;
				norm = { -dir.y, dir.x };
			}
			if (norm == vec2f(0, 0) || p12 - p21 < aln) {
				aln = p12 - p21;
				norm = { dir.y, -dir.x };
			}
		}

		if (norm == vec2f(0, 0)) {
			return Collision{};
		}

		Collision coll;
		coll.exists = true;
		coll.normal = -norm;
		coll.point = getCenter(convexIntersecton(poly1, opoly1));
		coll.depth = aln;
		return coll;
	}
};

#endif