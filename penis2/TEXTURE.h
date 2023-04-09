#ifndef __TEXTUREH__
#define __TEXTUREH__

#include "SHADER.hpp"
#include "TEXTURE.hpp"
#include "RENDERTEXTURE.hpp"

SPRG textureApplier;

void initShaders() {
	//textureApplier.loadFromFile("textureApplier.vert", "", "", "", "textureApplier.frag");
	textureApplier.loadFromFile("shader.vert", "", "", "", "shader.frag");
}

#endif // !__TEXTUREH__
