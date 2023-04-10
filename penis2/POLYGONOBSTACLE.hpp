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
		auto triangulation = PolygonTriangulator<FLOAT>::get(hitbox);
		float miny = hitbox[0].y, maxy = miny;
		float minx = hitbox[0].x, maxx = minx;
		auto t = *hitbox.begin();
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
			trarr.push(a, color, vec2f((a.x - minx) / xwidth, (a.y - miny) / ywidth));
			trarr.push(b, color, vec2f((b.x - minx) / xwidth, (b.y - miny) / ywidth));
			trarr.push(c, color, vec2f((c.x - minx) / xwidth, (c.y - miny) / ywidth));
		}
		trarr.create();
		arrset = TRUE;
	}

	BOOL arrset = FALSE;

public:

	ConvexPolygonObstacle(const Polygonf& hitbox, COLOR color = GLBLACK) : hitbox(hitbox), color(color) { }

	/// <summary>
	/// Constructor takes a convex polygon #hitbox, a texture #texture and a colorFilter, which is a color texture will be multiplicated
	/// </summary>
	ConvexPolygonObstacle(const Polygonf& hitbox, GLTXTR* texture, COLOR colorFilter = GLBLACK) : 
		hitbox(hitbox), texture(texture), color(colorFilter) { }

	void setTexture(GLTXTR* newTexture) {
		arrset = FALSE;
		texture = newTexture;
	}

	void setColor(COLOR newColor) {
		arrset = FALSE;
		color = newColor;
	}

	VOID draw(const Camera& camera) {
		if (!arrset) {
			setTrArr();
		}
		if (texture) {
			textureApplier.setUniform("tex", *texture);
			textureApplier.setActiveShader();
			trarr.draw(textureApplier, Transform::getMat(), camera);
			textureApplier.setInactive();
		}
		else {
			// ¬Œ“ ›“Œ“ ›À« œ–Œƒ≈À¿… » œŒÃ≈Õﬂ… TEXTUREAPPLIER
			// COLORAPPLIER ﬂ ¬ »Õ»“ÿ≈…ƒ≈– ”∆≈ œ–Œ »Õ”À
			//colorApplier.setUniform("color", color);
			colorApplier.setActiveShader();

		}
	}
};

#endif // !__POLYGONOBSTACLEHPP__
