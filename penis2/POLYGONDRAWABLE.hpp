#ifndef __POLYGONDRAWABLEHPP__
#define __POLYGONDRAWABLEHPP__

#include "MATH.h"
#include "TRANSFORM.hpp"
#include "TEXTURE.h"
#include "COLOR.h"
#include "SHADER.hpp"
#include "CAMERA.h"
#include "TRIANGLEARRAY.hpp"
#include <cassert>

class PolygonDrawable : public Transform {
private:
	Polygonf hitbox{};
	COLOR color = GLWHITE;
	GLTXTR* texture = nullptr;
	TRARR trarr{};

	void setTrArr() {
		trarr.clear();
		auto triangulation = PolygonTriangulator<FLOAT>::get(hitbox);
		float miny = hitbox[0].y, maxy = miny;
		float minx = hitbox[0].x, maxx = minx;
		for (const auto& vec : hitbox) {
			auto x = vec.x;
			auto y = vec.y;
			miny = min(miny, x);
			maxy = max(maxy, y);
			minx = min(minx, x);
			maxx = max(maxx, x);
		}
		float xwidth = maxx - minx;
		float ywidth = maxy - miny;
		for (const auto& [a, b, c] : triangulation) {
			if (texture != nullptr) {
				trarr.push(a, color, vec2f((a.x - minx) / xwidth, (a.y - miny) / ywidth));
				trarr.push(b, color, vec2f((b.x - minx) / xwidth, (b.y - miny) / ywidth));
				trarr.push(c, color, vec2f((c.x - minx) / xwidth, (c.y - miny) / ywidth));
			}
			else {
				trarr.push(a, color, {});
				trarr.push(b, color, {});
				trarr.push(c, color, {});
			}
		}
		trarr.create();
		arrset = TRUE;
	}

	BOOL arrset = FALSE;

public:

	PolygonDrawable(const Polygonf& hitbox, COLOR color = GLWHITE) : hitbox(hitbox), color(color) { }

	/// <summary>
	/// Constructor takes a convex polygon #hitbox, a texture #texture and a colorFilter, which is a color texture will be multiplicated
	/// </summary>
	PolygonDrawable(const Polygonf& hitbox, GLTXTR* texture, COLOR colorFilter = GLWHITE) : 
		hitbox(hitbox), texture(texture), color(colorFilter) { }

	void setTexture(GLTXTR* newTexture) {
		arrset = FALSE;
		texture = newTexture;
	}

	void setColor(COLOR newColor) {
		arrset = FALSE;
		color = newColor;
	}

	const Polygonf& box() const {
		return hitbox;
	}

	VOID draw(const Camera& camera) {
		if (!arrset) {
			setTrArr();
		}
		if (texture != nullptr) {
			basicDraw.setUniform("tex", *texture);
			basicDraw.setUniform("hasTexture", 1);
			trarr.draw(basicDraw, Transform::getMat(), camera);
		}
		else {
			basicDraw.setUniform("hasTexture", 0);
			trarr.draw(basicDraw, Transform::getMat(), camera);
		}
	}
};

#endif
