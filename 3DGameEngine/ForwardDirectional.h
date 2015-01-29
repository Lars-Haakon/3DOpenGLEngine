#ifndef FORWARDDIRECTIONAL_H
#define FORWARDDIRECTIONAL_H

#include "Shader.h"

class ForwardDirectional : public Shader
{
public:
	ForwardDirectional();

	virtual void updateUniforms(Matrix4f &worldMatrix, Matrix4f &value, Material *material);
private:
	DirectionalLight directionalLight;
};

#endif