#ifndef __GLLOADERHPP__
#define __GLLOADERHPP__

#include <Windows.h>

#define GL_ARRAY_BUFFER 0x8892
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0

typedef unsigned int GLuint;
typedef int GLsizei;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned short GLushort;
typedef int GLint;
typedef float GLfloat;
typedef unsigned char GLboolean;

typedef void*(*WGLGETPROCADDRESSPROC_PRIVATE)(const char*);
static WGLGETPROCADDRESSPROC_PRIVATE getProcAddressPtr;

typedef void(*GLGENBUFFERS)(GLsizei cnt, GLuint* buff);
GLGENBUFFERS _glGenBuffers;
#define glGenBuffers _glGenBuffers

typedef void(*GLGENVERTEXARRAYS)(GLsizei cnt, GLuint* buff);
GLGENVERTEXARRAYS _glGenVertexArrays;
#define glGenVertexArrays _glGenVertexArrays

typedef void(*GLBINDVERTEXARRAY)(GLuint arr);
GLBINDVERTEXARRAY _glBindVertexArray;
#define glBindVertexArray _glBindVertexArray

typedef void(*GLBINDBUFFER)(GLenum target, GLuint buff);
GLBINDBUFFER _glBindBuffer;
#define glBindBuffer _glBindBuffer

typedef void(*GLBUFFERDATA)(GLenum target, GLsizei size, const void* data, GLenum usage);
GLBUFFERDATA _glBufferData;
#define glBufferData _glBufferData

typedef void(*GLVERTEXATTRIBPOINTER)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
GLVERTEXATTRIBPOINTER _glVertexAttribPointer;
#define glVertexAttribPointer _glVertexAttribPointer

typedef void(*GLENABLEVERTEXATTRIBARRAY)(GLuint index);
GLENABLEVERTEXATTRIBARRAY _glEnableVertexAttribArray;
#define glEnableVertexAttribArray _glEnableVertexAttribArray

typedef void(*GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
GLVIEWPORT _glViewport;
#define glViewport _glViewport

typedef void(*GLCLEARCOLOR)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLCLEARCOLOR _glClearColor;
#define glClearColor _glClearColor

typedef void(*GLCLEAR)(GLbitfield mask);
GLCLEAR _glClear;
#define glClear _glClear



void* getAddress(const char* name) {
	void* p = (void*)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1)) {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, name);
	}

	return p;
}
void initOpenGl() {
	_glGenBuffers = (GLGENBUFFERS)getAddress("glGenBuffers");
	_glGenVertexArrays = (GLGENVERTEXARRAYS)getAddress("glGenVertexArrays");
	_glBindVertexArray = (GLBINDVERTEXARRAY)getAddress("glBindVertexArray");
	_glBindBuffer = (GLBINDBUFFER)getAddress("glBindBuffer");
	_glBufferData = (GLBUFFERDATA)getAddress("glBufferData");
	_glVertexAttribPointer = (GLVERTEXATTRIBPOINTER)getAddress("glVertexAttribPointer");
	_glEnableVertexAttribArray = (GLENABLEVERTEXATTRIBARRAY)getAddress("glEnableVertexAttribArray");
	_glViewport = (GLVIEWPORT)getAddress("glViewport");
	_glClearColor = (GLCLEARCOLOR)getAddress("glClearColor");
	_glClear = (GLCLEAR)getAddress("glClear");

	debug << _glViewport << std::endl;
}

#endif