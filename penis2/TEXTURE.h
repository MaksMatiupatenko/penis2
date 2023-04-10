#ifndef __TEXTUREH__
#define __TEXTUREH__

#include "SHADER.hpp"
#include "TEXTURE.hpp"
#include "RENDERTEXTURE.hpp"
#include "GLLOADER.hpp"

SPRG shader;
SPRG basicDraw;

void initShaders() {
	basicDraw.loadFromFile("basicDrawShader.vert", "", "", "", "basicDrawShader.frag");
	shader.loadFromFile("shader.vert", "", "", "", "shader.frag");
}


#endif // !__TEXTUREH__
