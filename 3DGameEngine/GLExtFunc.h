#ifndef GLEXTFUNC_H
#define GLEXTFUNC_H

#include <Windows.h>
#include <gl\GL.h>

extern void glExtFuncInit();

// Defines
#define GL_DEPTH_CLAMP 0x864F

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_STATIC_DRAW_ARB 0x88E4

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81

#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1

#define GL_TEXTURE0 0x84C0
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

// wgl Extension typedefes
typedef void (APIENTRY *PFNWGLGETEXTENSIONSSTRINGEXTPROC) (int vSync);

// VBO Extension typedefes
typedef void (APIENTRY *PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY *PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

// TEXTURE Extension typedefes
typedef void(APIENTRY *PFNGLACTIVETEXTURE) (GLenum texture);

// SHADER Extension typedefes
typedef void (APIENTRY *PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const char *name);
typedef GLuint(APIENTRY *PFNGLCREATEPROGRAMPROC) ();
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef GLuint(APIENTRY *PFNGLCREATESHADERPROC) (GLenum shaderType);
typedef void (APIENTRY *PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char **string, const GLint *length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei maxLength, GLsizei *length, char *infoLog);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC) (GLuint program);
typedef GLuint(APIENTRY *PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const char *name);
typedef void (APIENTRY *PFNGLUNIFORM1IPROC) (GLint location, GLint i0);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// wgl Extension Function Pointers
extern PFNWGLGETEXTENSIONSSTRINGEXTPROC wglSwapIntervalEXT;

// VBO Extension Function Pointers
extern PFNGLGENBUFFERSPROC glGenBuffersARB;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERPROC glBindBufferARB;					// VBO Bind Procedure
extern PFNGLBUFFERDATAPROC glBufferDataARB;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSPROC glDeleteBuffersARB;			// VBO Deletion Procedure
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArrayARB;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointerARB;

// TEXTURE Extension Function Pointers
extern PFNGLACTIVETEXTURE glActiveTexture;

// SHADER Extension Function Pointers
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;


#endif