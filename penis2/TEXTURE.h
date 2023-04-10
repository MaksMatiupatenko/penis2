#ifndef __TEXTUREH__
#define __TEXTUREH__

#include "SHADER.hpp"
#include "TEXTURE.hpp"
#include "RENDERTEXTURE.hpp"
#include "GLLOADER.hpp"

SPRG textureApplier;
SPRG colorApplier;

void initShaders() {
	colorApplier.loadFromFile("colorApplier.vert", "", "", "", "colorApplier.frag");
	textureApplier.loadFromFile("shader.vert", "", "", "", "shader.frag");
}

struct _GL_INITIALIZER {
	_GL_INITIALIZER() {
		initOpenGl();
		initShaders();
	}
} GL_INITIALIZER;


#endif // !__TEXTUREH__
