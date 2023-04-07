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
		debug << "hui" << std::endl;
		glGenTextures(1, &id);
		debug << "hui" << std::endl;
		glBindTexture(GL_TEXTURE_2D, id);
		debug << "hui" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
		debug << "hui" << std::endl;
		glGenerateMipmap(GL_TEXTURE_2D);
		debug << "hui" << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		debug << "hui" << std::endl;
	}

	VOID use() {
		glBindTexture(GL_TEXTURE_2D, id);
	}

	VOID open(const std::string& path, GLenum textureFormat) {
		stbi_set_flip_vertically_on_load(1);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		for (int i = 0; i < 1000; ++i) {
			debug << data[i];
		}
		debug << std::endl;

		debug << "hui" << std::endl;
		GLenum dataFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		create(textureFormat, width, height, dataFormat[nrChannels - 1], data);
		debug << "hui" << std::endl;

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
};

#endif