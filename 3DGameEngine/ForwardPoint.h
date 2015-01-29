#ifndef FORWARDPOINT_H
#define FORWARDPOINT_H

#include "Shader.h"

class ForwardPoint : public Shader
{
public:
	ForwardPoint();

	virtual void updateUniforms(Matrix4f &worldMatrix, Matrix4f &value, Material *material);
private:
	PointLight pointLight;
};

#endif