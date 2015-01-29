#include "shader.h"
#include "glExtFunc.h"

#include "Console.h"

Shader::Shader()
{
	uniforms = new HashTable<int>(17);

	m_program = glCreateProgram();

	if (m_program == 0)
	{
		Console::printf("%s\n", "Error creating shader program\n");
		exit(1);
	}
}

Shader::~Shader()
{
	delete uniforms;

	for (int i = 0; i < shaderList.getLength(); i++)
	{
		int *shader = shaderList.get(i);

		glDetachShader(m_program, *shader);
		glDeleteShader(*shader);
	}

	glDeleteProgram(m_program);
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::updateUniforms(Matrix4f &worldMatrix, Matrix4f& value, Material *material)
{

}

void Shader::compileShader()
{
	glLinkProgram(m_program);
}

void Shader::addUniform(char *uniform)
{
	int *uniformLocation = new int;
	*uniformLocation = glGetUniformLocation(m_program, uniform);

	if (*uniformLocation != -1)
	{
		uniforms->put(uniform, uniformLocation);
	}
	else
	{
		delete uniformLocation;
		Console::printf("%s %s\n", "UNIFORM ERROR", uniform);
		system("PAUSE");
		exit(1);
	}
}

void Shader::addVertexShader(SourceInfo info)
{
	addProgram(info, GL_VERTEX_SHADER);
}

void Shader::addFragmentShader(SourceInfo info)
{
	addProgram(info, GL_FRAGMENT_SHADER);
}

void Shader::setAttribLocation(int location, char *attribName)
{
	glBindAttribLocation(m_program, location, attribName);
}

void Shader::addProgram(SourceInfo info, int type)
{
	int *shader = new int; 
	*shader = glCreateShader(type);

	if (shader == 0)
	{
		Console::printf("%s %d\n", "Error creating shader type", type);
		exit(1);
	}

	const char *source = info.source;

	glShaderSource(*shader, 1, &source, &info.length);
	delete[] info.source;

	glCompileShader(*shader);

	GLint success;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(*shader, 1024, NULL, infoLog);
		Console::printf("Error compiling shader type %d %s\n", shader, infoLog);
		system("PAUSE");
		exit(1);
	}

	glAttachShader(m_program, *shader);
	shaderList.add(shader);
}

SourceInfo Shader::loadShaderSource(const char *fileName)
{
	char *source = NULL;
	FILE *fp = NULL;
	fopen_s(&fp, fileName, "r");

	if (fp != NULL)
	{
		if (fseek(fp, 0L, SEEK_END) == 0)
		{
			/* Get the size of the file. */
			int bufsize = ftell(fp);
			rewind(fp);
			/* Allocate our buffer to that size. */
			source = new char[bufsize + 1];

			/* Read the entire file into memory. */
			int newLen = fread(source, 1, bufsize, fp);
			fclose(fp);
			if (newLen == 0)
			{
				Console::printf("%s\n", "Error reading file");
				exit(1);
			}

			source[newLen] = '\0';

			SourceInfo info;
			info.source = source;
			info.length = newLen;

			return info;
		}
		else
			exit(1);

		Console::printf("%s\n", source);
	}
	else
	{
		Console::printf("%s\n", "Could not open file");
		exit(1);
	}
}