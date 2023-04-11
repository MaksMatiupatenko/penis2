#ifndef __SHADOWDRAWERHPP__
#define __SHADOWDRAWERHPP__

#include "TRIANGLEARRAY.hpp"
#include "RENDERTEXTURE.hpp"
#include "POLYGONOBSTACLE.hpp"
#include "MATH.h"
#include "CAMERA.h"
#include "TEXTURE.h"

class ShadowDrawer {
private:
	GLRTXTR renderTexture;
	TRARR trarr;
	vec4f col;
	
public:
	void create(int width, int height) {
		renderTexture.create();
		renderTexture.createTexture(GL_RGBA, width, height);
	}

	void clear() {
		trarr.clear();
	}

	void setShadowColor(vec4f color) {
		col = color;
	}

	void destruct() {
		clear();
		renderTexture.destruct();
	}

	void push(const vec2f& pos, const vec2f& a, const vec2f& b) {
		vec2f t1 = normalize(a - pos) * 1e9f;
		vec2f t2 = normalize(b - pos) * 1e9f;
		vec4f c = vec4f(col.x, col.y, col.z, 1);
		trarr.push(a, c, vec4f());
		trarr.push(b, c, vec4f());
		trarr.push(t1, c, vec4f());
		trarr.push(b, c, vec4f());
		trarr.push(t1, c, vec4f());
		trarr.push(t2, c, vec4f());
	}

	void push(vec2f pos, const Polygonf& poly) {
		for (int i = 0; i < poly.size(); ++i) {
			push(pos, poly.get(i), poly.get(i + 1));
		}
	}

	void push(vec2f pos, PolygonDrawable* poly) {
		push(pos, poly->box());
	}

	void draw(const Camera& camera) {
		trarr.create();

		renderTexture.bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		basicDraw.setUniform("hasTexture", 0);
		trarr.draw(basicDraw, mat3f(), camera);
		renderTexture.unbind();

		TRARR trarr1;
		vec4f c(1, 1, 1, col.w);
		trarr1.push(vec2f(-1, -1), c, vec2f(0, 0));
		trarr1.push(vec2f(-1, 1), c, vec2f(0, 1));
		trarr1.push(vec2f(1, -1), c, vec2f(1, 0));
		trarr1.push(vec2f(1, 1), c, vec2f(1, 1));
		trarr1.push(vec2f(-1, 1), c, vec2f(0, 1));
		trarr1.push(vec2f(1, -1), c, vec2f(1, 0));
		trarr1.create();

		basicDraw.setUniform("hasTexture", 1);
		basicDraw.setUniform("tex", renderTexture.getTexture());
		trarr1.draw(basicDraw, mat3f(), Camera());

		trarr1.clear();
	}
};

#endif
