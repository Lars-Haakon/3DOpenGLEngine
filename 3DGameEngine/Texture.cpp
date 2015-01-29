#include "Texture.h"
#include <stdio.h>
#include "Console.h"
#include "glExtFunc.h"

Texture::Texture()
{
	glGenTextures(1, &m_id);
}

Texture::Texture(char *data)
{
	glGenTextures(1, &m_id);
	bind();

	//Setup wrap mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setup texture scaling filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

GLuint Texture::getID()
{
	return m_id;
}

Texture *Texture::loadBMP(const char *fileName)
{
	FILE *file = NULL;
	fopen_s(&file, fileName, "rb");

	if (file == NULL)
	{
		Console::printf("%s\n", "Could not open file");
		exit(1);
	}

	char header[54];

	//read in header
	if (fread(header, 1, 54, file) != 54)
	{
		Console::printf("%s\n", "Wrong texture format");
		exit(1);
	}

	int dataPos = *(int*)&(header[0xA]);
	int imageSize = *(int*)&(header[0x22]);
	int width = *(int*)&(header[0x12]);
	int height = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54;

	// allocate buffer
	char *data = new char[imageSize];

	//read texture data
	int readSize = fread(data, 1, imageSize, file);
	fclose(file);

	Texture *ret = new Texture();
	ret->bind();

	//Setup wrap mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setup texture scaling filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	delete[] data;

	return ret;
}