#ifndef FORWARDSPOT_H
#define FORWARDSPOT_H

#include "Shader.h"

class ForwardSpot : public Shader
{
public:
	ForwardSpot();

	virtual void updateUniforms(Matrix4f &worldMatrix, Matrix4f &value, Material *material);
private:
	SpotLight spotLight;
};

#endif