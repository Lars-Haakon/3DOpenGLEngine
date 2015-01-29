#ifndef FORWARDAMBIENT_H
#define FORWARDAMBIENT_H

#include "Shader.h"

class ForwardAmbient : public Shader
{
public:
	ForwardAmbient();

	virtual void updateUniforms(Matrix4f &worldMatrix, Matrix4f &projectedMatrix, Material *material);
};

#endif