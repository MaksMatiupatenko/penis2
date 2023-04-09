#ifndef __TEXTUREHPP__
#define __TEXTUREHPP__

#include "BASE.h"
#include "GLLOADER.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include <string>

class GLTXTR {
private:
	UINT _id = 0;

public:
	GLTXTR() = default;

	VOID create(GLenum textureFormat, int width, int height, GLenum dataFormat, const void* pixels) {
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	VOID use() CONST {
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	VOID open(const std::string& path, GLenum textureFormat) {
		stbi_set_flip_vertically_on_load(1);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		GLenum dataFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		create(textureFormat, width, height, dataFormat[nrChannels - 1], data);

		stbi_image_free(data);
	}

	VOID save(const std::string& path) {
		stbi_flip_vertically_on_write(1);

		glBindTexture(GL_TEXTURE_2D, _id);
		int width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		unsigned char* pixels = new unsigned char[4ll * width * height];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		stbi_write_png(path.c_str(), width, height, 4, pixels, sizeof(unsigned char) * width * 4);

		delete[] pixels;
	}

	VOID destruct() {
		glDeleteTextures(1, &_id);
	}

	UINT id() {
		return _id;
	}
};

#endif