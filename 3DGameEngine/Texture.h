#ifndef TEXTURE_H
#define TEXTURE_H

#include <Windows.h>
#include <gl\GL.h>

class Texture
{
public:
	Texture();
	Texture(char *data);
	~Texture();

	void bind();

	static Texture *loadBMP(const char *fileName);

	GLuint getID();
private:
	GLuint m_id;
};

#endif