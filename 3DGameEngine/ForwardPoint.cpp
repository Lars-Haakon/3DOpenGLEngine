#include "ForwardPoint.h"
#include "GLExtFunc.h"
#include "Transform.h"

ForwardPoint::ForwardPoint()
{
	addVertexShader(Shader::loadShaderSource("shaders/forward-point.vs"));
	addFragmentShader(Shader::loadShaderSource("shaders/forward-point.fs"));
	compileShader();

	setAttribLocation(0, "position");
	setAttribLocation(1, "normal");
	setAttribLocation(2, "tangent");
	setAttribLocation(3, "texCoord");

	addUniform("model");
	addUniform("MVP");
	addUniform("eyePos");
	addUniform("pointLight.base.color");
	addUniform("pointLight.base.intensity");
	addUniform("pointLight.atten.constant");
	addUniform("pointLight.atten.linear");
	addUniform("pointLight.atten.exponent");
	addUniform("pointLight.position");
	addUniform("specularIntensity");
	addUniform("specularPower");
	addUniform("colorMap");
	addUniform("normalMap");

	pointLight.base.color = Vector3f(1, 1, 1);
	pointLight.base.intensity = 300.0f;
	pointLight.atten.constant = 0;
	pointLight.atten.linear = 0;
	pointLight.atten.exponent = 0.01f;
	pointLight.position = Vector3f(0, 120, 0);
}

void ForwardPoint::updateUniforms(Matrix4f &worldMatrix, Matrix4f &projectedMatrix, Material *material)
{
	glActiveTexture(GL_TEXTURE0);
	material->getColorMap()->bind();
	glUniform1i(*uniforms->get("colorMap"), 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	material->getNormalMap()->bind();
	glUniform1i(*uniforms->get("normalMap"), 1);

	glUniformMatrix4fv(*uniforms->get("model"), 1, GL_TRUE, &(worldMatrix[0][0]));
	glUniformMatrix4fv(*uniforms->get("MVP"), 1, GL_TRUE, &(projectedMatrix[0][0]));

	Vector3f pos = Transform::getCamera()->getPos();
	glUniform3f(*uniforms->get("eyePos"), pos.getX(), pos.getY(), pos.getZ());
	glUniform3f(*uniforms->get("pointLight.base.color"), pointLight.base.color.getX(), pointLight.base.color.getY(), pointLight.base.color.getZ());
	glUniform1f(*uniforms->get("pointLight.base.intensity"), pointLight.base.intensity);
	glUniform1f(*uniforms->get("pointLight.atten.constant"), pointLight.atten.constant);
	glUniform1f(*uniforms->get("pointLight.atten.linear"), pointLight.atten.linear);
	glUniform1f(*uniforms->get("pointLight.atten.exponent"), pointLight.atten.exponent);
	glUniform3f(*uniforms->get("pointLight.position"), pointLight.position.getX(), pointLight.position.getY(), pointLight.position.getZ());
	glUniform1f(*uniforms->get("specularIntensity"), material->getSpecularIntensity());
	glUniform1f(*uniforms->get("specularPower"), material->getSpecularPower());
}
