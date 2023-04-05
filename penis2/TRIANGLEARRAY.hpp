#ifndef __TRIANGLEARRAYHPP__
#define __TRIANGLEARRAYHPP__

#include "BASE.h"
#include "MATH.h"
#include <vector>
#include "GLLOADER.hpp"


#define DYNARR std::vector

//TODO: write own vector

class TRARR {
private:
	DYNARR <FLOAT> data;

	UINT vao, vbo;

#define ATTRIBSINVERTEX 12

public:
	VOID push(vec4f coord, vec4f color, vec4f texcoord) {
		data.push_back(coord.x);
		data.push_back(coord.y);
		data.push_back(coord.z);
		data.push_back(coord.w);
		data.push_back(color.r);
		data.push_back(color.g);
		data.push_back(color.b);
		data.push_back(color.a);
		data.push_back(texcoord.x);
		data.push_back(texcoord.y);
		data.push_back(texcoord.z);
		data.push_back(texcoord.w);
	}

	VOID create() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ATTRIBSINVERTEX * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

#undef DYNARR

#endif