#pragma once

#include "COLLIDER.hpp"
#include "DRAWABLE.hpp"

struct RigidBody : public Transform {
private:
	float mass{};
	float invMass{};
	Drawable* drawable{};

public:
	vec2f velocity{};
	float restitution{};
	Collider* collider{};
	
	RigidBody() = default;

	/// <summary>
	/// Если массу в 0 поставить, то объект не будет двигаться никогда
	/// </summary>
	/// <param name="_mass"></param>
	/// <param name="_restitution"></param>
	/// <param name="_collider"></param>
	/// <param name="drawable"></param>
	RigidBody(
		float _mass, float _restitution,
		Collider* _collider,
		Drawable* _drawable = nullptr) {
		setMass(_mass);
		restitution = _restitution;
		setCollider(_collider);
		setDrawModel(_drawable);
	}

	void setCollider(Collider* newCol) {
		collider = newCol;
		if (collider) {
			collider->base = (Transform*)this;
		}
	}

	void setDrawModel(Drawable* newDr) {
		drawable = newDr;
		if (drawable) {
			drawable->baseTransform = (Transform*)this;
		}
	}

	void setMass(float newMass) {
		mass = newMass;
		if (mass != 0) {
			invMass = 1 / mass;
		}
		else {
			invMass = 0;
		}
	}

	void makeMassInfinite() {
		setMass(0);
	}

	float getMass() const {
		return mass;
	}

	float getInvMass() const {
		return invMass;
	}

	void draw(const Camera& camera) const {
		if (drawable) {
			return drawable->draw(camera);
		}
	}
};

void resolveCollision(RigidBody& A, RigidBody& B) {
	auto [collide, normal, depth] = A.collider->get(B.collider);
	if (!collide) {
		return;
	}
	vec2f rv = -B.velocity + A.velocity;
	float velAlongNormal = dt(rv, normal);
	if (velAlongNormal > 0) {
		return;
	}
	float e = min(A.restitution, B.restitution);
	float j = -(1 + e) * velAlongNormal;
	j /= A.getInvMass() + B.getInvMass();
	vec2f impulse = normal * j;
	A.velocity += impulse * A.getInvMass();
	B.velocity -= impulse * B.getInvMass();
}