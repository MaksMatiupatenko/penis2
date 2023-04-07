#ifndef __GLLOADERHPP__
#define __GLLOADERHPP__

#include <Windows.h>

#define GL_ARRAY_BUFFER 0x8892
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRIANGLES 0x0004
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01

typedef unsigned int GLuint;
typedef int GLsizei;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned short GLushort;
typedef int GLint;
typedef float GLfloat;
typedef unsigned char GLboolean;
typedef char GLchar;

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

typedef void(*GLDRAWARRAYS)(GLenum mode, GLint first, GLsizei count);
GLDRAWARRAYS _glDrawArrays;
#define glDrawArrays _glDrawArrays

typedef GLuint(*GLCREATESHADER)(GLenum type);
GLCREATESHADER _glCreateShader;
#define glCreateShader _glCreateShader

typedef void(*GLSHADERSOURCE)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
GLSHADERSOURCE _glShaderSource;
#define glShaderSource _glShaderSource

typedef void(*GLCOMPILESHADER)(GLuint shader);
GLCOMPILESHADER _glCompileShader;
#define glCompileShader _glCompileShader

typedef void(*GLDELETEPROGRAM)(GLuint program);
GLDELETEPROGRAM _glDeleteProgram;
#define glDeleteProgram _glDeleteProgram

typedef GLuint(*GLCREATEPROGRAM)(void);
GLCREATEPROGRAM _glCreateProgram;
#define glCreateProgram _glCreateProgram

typedef void(*GLATTACHSHADER)(GLuint program, GLuint shader);
GLATTACHSHADER _glAttachShader;
#define glAttachShader _glAttachShader

typedef void(*GLDELETESHADER)(GLuint shader);
GLDELETESHADER _glDeleteShader;
#define glDeleteShader _glDeleteShader

typedef void (*GLLINKPROGRAM)(GLuint program);
GLLINKPROGRAM _glLinkProgram;
#define glLinkProgram _glLinkProgram

typedef void(*GLUSEPROGRAM)(GLuint program);
GLUSEPROGRAM _glUseProgram;
#define glUseProgram _glUseProgram

typedef GLint(*GLGETUNIFORMLOCATION)(GLuint program, const GLchar* name);
GLGETUNIFORMLOCATION _glGetUniformLocation;
#define glGetUniformLocation _glGetUniformLocation

typedef void(*GLUNIFORM1F)(GLint location, GLfloat v0);
GLUNIFORM1F _glUniform1f;
#define glUniform1f _glUniform1f

typedef void(*GLGETSHADERIV)(GLuint shader, GLenum pname, GLint* params);
GLGETSHADERIV _glGetShaderiv;
#define glGetShaderiv _glGetShaderiv

typedef void(*GLGETSHADERINFOLOG)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLGETSHADERINFOLOG _glGetShaderInfoLog;
#define glGetShaderInfoLog _glGetShaderInfoLog

typedef void(*GLGETPROGRAMIV)(GLuint program, GLenum pname, GLint* params);
GLGETPROGRAMIV _glGetProgramiv;
#define glGetProgramiv _glGetProgramiv

typedef void(*GLGETPROGRAMINFOLOG)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLGETPROGRAMINFOLOG _glGetProgramInfoLog;
#define glGetProgramInfoLog _glGetProgramInfoLog

typedef void(*GLDELETEVERTEXARRAYS)(GLsizei n, const GLuint* arrays);
GLDELETEVERTEXARRAYS _glDeleteVertexArrays;
#define glDeleteVertexArrays _glDeleteVertexArrays

typedef void(*GLDELETEBUFFERS)(GLsizei n, const GLuint* buffers);
GLDELETEBUFFERS _glDeleteBuffers;
#define glDeleteBuffers _glDeleteBuffers

typedef void(*GLUNIFORMMATRIX3FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLUNIFORMMATRIX3FV _glUniformMatrix3fv;
#define glUniformMatrix3fv _glUniformMatrix3fv

typedef void(*GLGENTEXTURES)(GLsizei n, GLuint* textures);
GLGENTEXTURES _glGenTextures;
#define glGenTextures _glGenTextures

typedef void(*GLBINDTEXTURE)(GLenum target, GLuint texture);
GLBINDTEXTURE _glBindTexture;
#define glBindTexture _glBindTexture

typedef void(*GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
GLTEXIMAGE2D _glTexImage2D;
#define glTexImage2D _glTexImage2D

typedef void(*GLGENERATEMIPMAP)(GLenum target);
GLGENERATEMIPMAP _glGenerateMipmap;
#define glGenerateMipmap _glGenerateMipmap

typedef void(*GLGETTEXLEVELPARAMETERIV)(GLenum target, GLint level, GLenum pname, GLint* params);
GLGETTEXLEVELPARAMETERIV _glGetTexLevelParameteriv;
#define glGetTexLevelParameteriv _glGetTexLevelParameteriv

typedef void(*GLGETTEXIMAGE)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
GLGETTEXIMAGE _glGetTexImage;
#define glGetTexImage _glGetTexImage

typedef void(*GLACTIVETEXTURE)(GLenum texture);
GLACTIVETEXTURE _glActiveTexture;
#define glActiveTexture _glActiveTexture



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
	_glDrawArrays = (GLDRAWARRAYS)getAddress("glDrawArrays");
	_glCreateShader = (GLCREATESHADER)getAddress("glCreateShader");
	_glShaderSource = (GLSHADERSOURCE)getAddress("glShaderSource");
	_glCompileShader = (GLCOMPILESHADER)getAddress("glCompileShader");
	_glDeleteProgram = (GLDELETEPROGRAM)getAddress("glDeleteProgram");
	_glCreateProgram = (GLCREATEPROGRAM)getAddress("glCreateProgram");
	_glAttachShader = (GLATTACHSHADER)getAddress("glAttachShader");
	_glDeleteShader = (GLDELETESHADER)getAddress("glDeleteShader");
	_glLinkProgram = (GLLINKPROGRAM)getAddress("glLinkProgram");
	_glUseProgram = (GLUSEPROGRAM)getAddress("glUseProgram");
	_glGetUniformLocation = (GLGETUNIFORMLOCATION)getAddress("glGetUniformLocation");
	_glUniform1f = (GLUNIFORM1F)getAddress("glUniform1f");
	_glGetShaderiv = (GLGETSHADERIV)getAddress("glGetShaderiv");
	_glGetShaderInfoLog = (GLGETSHADERINFOLOG)getAddress("glGetShaderInfoLog");
	_glGetProgramiv = (GLGETPROGRAMIV)getAddress("glGetProgramiv");
	_glGetProgramInfoLog = (GLGETPROGRAMINFOLOG)getAddress("glGetProgramInfoLog");
	_glDeleteVertexArrays = (GLDELETEVERTEXARRAYS)getAddress("glDeleteVertexArrays");
	_glDeleteBuffers = (GLDELETEBUFFERS)getAddress("glDeleteBuffers");
	_glUniformMatrix3fv = (GLUNIFORMMATRIX3FV)getAddress("glUniformMatrix3fv");
	_glGenTextures = (GLGENTEXTURES)getAddress("glGenTextures");
	_glBindTexture = (GLBINDTEXTURE)getAddress("glBindTexture");
	_glTexImage2D = (GLTEXIMAGE2D)getAddress("glTexImage2D");
	_glGenerateMipmap = (GLGENERATEMIPMAP)getAddress("glGenerateMipmap");
	_glGetTexLevelParameteriv = (GLGETTEXLEVELPARAMETERIV)getAddress("glGetTexLevelParameteriv");
	_glGetTexImage = (GLGETTEXIMAGE)getAddress("glGetTexImage");
	_glActiveTexture = (GLACTIVETEXTURE)getAddress("glActiveTexture");
}

#endif