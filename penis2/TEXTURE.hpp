#ifndef __TEXTUREHPP__
#define __TEXTUPEHPP__

#include "BASE.h"
#include "GLLOADER.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>

class GLTXTR {
private:
	
	UINT id;

public:
	VOID create(GLenum textureFormat, int width, int height, GLenum dataFormat, const void* pixels) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	VOID use() {
		glBindTexture(GL_TEXTURE_2D, id);
	}

	VOID open(const std::string& path, GLenum textureFormat) {
		stbi_set_flip_vertically_on_load(1);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		create(textureFormat, width, height, GL_RGBA, data);

		stbi_image_free(data);
	}

	VOID save(const std::string& path) {
		stbi_flip_vertically_on_write(1);

		glBindTexture(GL_TEXTURE_2D, id);
		int width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		unsigned char* pixels = new unsigned char[4ll * width * height];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		stbi_write_png(path.c_str(), width, height, 4, pixels, sizeof(unsigned char) * width * 4);

		delete[] pixels;
	}

	UINT getid() CONST {
		return id;
	}
};

#endif