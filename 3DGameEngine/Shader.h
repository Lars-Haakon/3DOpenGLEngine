#ifndef SHADER_H
#define SHADER_H

#include "LinkedList.h"
#include "HashTable.h"
#include "Matrix4f.h"
#include "Material.h"

struct BaseLight
{
	Vector3f color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	Vector3f direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	Vector3f position;
};

struct SpotLight
{
	PointLight point;
	Vector3f direction;
	float cutoff;
};

struct SourceInfo
{
	char *source;
	int length;
};

struct UniformInfo
{
	char *uniform;
	int location;
};

class Shader
{
public:
	Shader();
	~Shader();

	void bind();
	virtual void updateUniforms(Matrix4f &worldMatrix, Matrix4f& value, Material *material) = 0;

	static SourceInfo loadShaderSource(const char *fileName);
protected:
	LinkedList<int> shaderList;
	HashTable<int> *uniforms;

	void addUniform(char *uniform);
	void addVertexShader(SourceInfo info);
	void addFragmentShader(SourceInfo info);

	void compileShader();
	void setAttribLocation(int location, char *attribName);
private:
	int m_program;

	void addProgram(SourceInfo info, int type);
};

#endif