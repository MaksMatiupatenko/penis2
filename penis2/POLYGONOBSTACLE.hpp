#ifndef __POLYGONOBSTACLEHPP__
#define __POLYGONOBSTACLEHPP__

#include "MATH.h"
#include "TRANSFORM.hpp"
#include "TEXTURE.h"
#include "COLOR.h"
#include "SHADER.hpp"
#include "CAMERA.h"
#include "TRIANGLEARRAY.hpp"

class ConvexPolygonObstacle : public Transform {
private:
	Polygonf hitbox;
	COLOR color;
	GLTXTR* texture = nullptr;
	TRARR trarr;

	void setTrArr() {
		trarr.clear();
		for (const auto& point : hitbox) {
			trarr.push(point, GLBLACK, vec2f());
		}
		trarr.create();
	}

public:

	ConvexPolygonObstacle(const Polygonf& hitbox, COLOR color = GLWHITE) : hitbox(hitbox), color(color) { }

	/// <summary>
	/// Constructor takes a convex polygon #hitbox, a texture #texture and a colorFilter, which is a color texture will be multiplicated
	/// </summary>
	ConvexPolygonObstacle(const Polygonf& hitbox, GLTXTR* texture, COLOR colorFilter = GLWHITE) : 
		hitbox(hitbox), texture(texture), color(colorFilter) { }


	/// <summary>
	/// я тут короче бля пока цвет не реализовал эта хуйня ток текстуру класть умеет если че. а еще техкорд сам туда положи заебал
	/// </summary>
	VOID draw(const Camera& camera) {
		trarr.draw(textureApplier, Transform::getMat(), camera);
	}
};

#endif // !__POLYGONOBSTACLEHPP__
