#ifndef __TRIANGLEARRAYHPP__
#define __TRIANGLEARRAYHPP__

#include "BASE.h"
#include "MATH.h"
#include <vector>
#include "GLLOADER.hpp"
#include "SHADER.hpp"
#include "CAMERA.hpp"
#include "CAMERA.h"
#include "COLOR.h"


#define DYNARR std::vector

//TODO: write own vector

class TRARR {
private:
	DYNARR <FLOAT> data;

	UINT vao = 0, vbo = 0;

#define ATTRIBSINVERTEX 12

public:
	~TRARR() {
		clear();
	}

	VOID push(vec4f coord, COLOR color, vec4f texcoord) {
		data.push_back(coord.x);
		data.push_back(coord.y);
		data.push_back(coord.z);
		data.push_back(coord.w);

		data.push_back(color.x);
		data.push_back(color.y);
		data.push_back(color.z);
		data.push_back(color.w);

		data.push_back(texcoord.x);
		data.push_back(texcoord.y);
		data.push_back(texcoord.z);
		data.push_back(texcoord.w);
	}

	VOID create() {
		if (vao != 0) {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, ATTRIBSINVERTEX * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, ATTRIBSINVERTEX * sizeof(GLfloat), (void*)(0+TRVWIDTH));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, ATTRIBSINVERTEX * sizeof(GLfloat), (void*)(0+TRVWIDTH+TRVWIDTH));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
	}

	VOID draw(SPRG& sprg, mat3f model, const Camera& camera) {
		sprg.setUniform("model", model);
		sprg.setUniform("view", camera.getMat());
		sprg.setActiveShader();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, data.size() / ATTRIBSINVERTEX);
		glBindVertexArray(NULL);
		sprg.setInactive();
	}

	VOID clear() {
		data.clear();
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
};

#undef DYNARR

#endif