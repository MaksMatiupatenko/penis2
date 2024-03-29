#pragma once

#include "COLLIDER.hpp"
#include "DRAWABLE.hpp"
#include "DRAWHUI.h"
#include "GRAVITY.h"
#include <iomanip>

struct RigidBody : public Transform {
private:
	float mass{};
	float invMass{};
	float rotMass{};
	float invRotMass{};
	Drawable* drawable{};
	vec2f center{};

public:
	vec2f velocity{};
	float rotVel{};
	/// óìîëÿþ íàçîâè ýòî ïîëå ñàìûì åáàíóòûìì àíãëèéñêèì ñëîâîì êîòîðîå çíàåøü
	/// 7-8 áëÿÿÿÿÿÿÿòü
	/// 
	/// ñýð åñòü ñýð ïîøåë íàõóé ìóäåíü
	float restitution{};
	Collider* collider{};
	
	RigidBody() = default;

	/// Åñëè ìàññó â 0 ïîñòàâèòü, òî îáúåêò íå áóäåò äâèãàòüñÿ íèêîãäà
	RigidBody(
		float _mass, float _restitution,
		Collider* _collider,
		Drawable* _drawable = nullptr) {
		setMass(_mass);
		setRotMass(_mass);
		restitution = _restitution;
		setCollider(_collider);
		setDrawModel(_drawable);
		center = _collider->center();
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

	void setRotMass(float newMass) {
		rotMass = newMass;
		if (rotMass != 0) {
			invRotMass = 1 / rotMass;
		}
		else {
			invRotMass = 0;
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

	Drawable* getDrawable() const {
		return drawable;
	}

	void update(float timeDiff) {
		if (invMass != 0) {
			velocity += gravity * timeDiff;
		}
		else {
			velocity = { 0, 0 };
			rotVel = 0;
		}
		Transform::absMove(velocity * timeDiff);
		Transform::rotate(rotVel * timeDiff);

		velocity /= exp(timeDiff * 0.1);
		rotVel /= exp(timeDiff * 0.1);
	}

	vec2f getPointVel(vec2f pos) const {
		return velocity + vec2f(-pos.y, pos.x) * rotVel;
	}

	float getPointDirInvMass(vec2f pos, vec2f dir) const {
		return invMass;
	}

	void addImpulse(vec2f pos, vec2f imp) {
		velocity += imp * invMass;
		/*float ln = length(pos);
		pos = normalize(pos);
		velocity += pos * dot(pos, imp);
		pos = { -pos.y, pos.x };
		//rotVel += dot(pos, vel) / ln;*/
	}
};

void resolveCollision(RigidBody& A, RigidBody& B) {
	if (!A.getInvMass() && !B.getInvMass()) {
		return;
	}
	auto [collide, normal, pos, depth] = A.collider->get(B.collider);
	if (!collide) {
		return;
	}

	debug << std::fixed << std::setprecision(10) << normal.x << ' ' << normal.y << std::endl;

	//vec2f posa = A.getRMat() * pos;
	//vec2f posb = B.getRMat() * pos;

	auto m1 = A.getInvMass();
	auto m2 = B.getInvMass();
	const float hui = 0.9;

	vec2f rv = -B.velocity + A.velocity;
	float velAlongNormal = dot(rv, normal);
	if (velAlongNormal < 0) {
		float e = min(A.restitution, B.restitution);
		float j = -(1 + e) * velAlongNormal;
		j /= m1 + m2;
		vec2f impulse = normal * j;
		A.addImpulse({}, impulse);
		B.addImpulse({}, -impulse);
	}

	A.absMove(normal * hui * depth * m1 / (m1 + m2));
	B.absMove(-normal * hui * depth * m2 / (m1 + m2));

	Polygonf strelka;
	strelka.push(0.1, -1);
	strelka.push(0.1, 0.6);
	strelka.push(0.4, 0.6);
	strelka.push(0.0, 1);
	strelka.push(-0.4, 0.6);
	strelka.push(-0.1, 0.6);
	strelka.push(-0.1, -1);

	Drawable dr(strelka, GLYELLOW);
	dr.setPos(pos);
	dr.setScale(0.1);
	dr.setAngle(atan2(-normal.x, normal.y));


	drawhui.push_back(dr);
}