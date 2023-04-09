#ifndef __TEXTUREH__
#define __TEXTUREH__

#include "SHADER.hpp"
#include "TEXTURE.hpp"
#include "RENDERTEXTURE.hpp"

SPRG textureApplier;

void initShaders() {
	textureApplier.loadFromFile("textureApplier.vert", "", "", "", "textureApplier.frag");
}

#endif // !__TEXTUREH__
