#ifndef __RENDERTEXTUREHPP__
#define __RENDERTEXTUREHPP__

#include "TEXTURE.hpp"
#include "BASE.h"

class GLRTXTR {
private:
	UINT fbo = 0;
	GLTXTR tex;

public:
    void create() {
        glGenFramebuffers(1, &fbo);
    }

    void destruct() {
        glDeleteFramebuffers(1, &fbo);
    }

    void createTexture(GLenum textureFormat, int width, int height) {
        tex.create(textureFormat, width, height, GL_RGBA, nullptr);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bindTexture(GLTXTR _tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        tex = _tex;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLTXTR getTexture() {
        return tex;
    }

    void deleteTexture() {
        tex.destruct();
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

#endif
