#include "ForwardSpot.h"
#include "GLExtFunc.h"
#include "Transform.h"

ForwardSpot::ForwardSpot()
{
	addVertexShader(Shader::loadShaderSource("shaders/forward-spot.vs"));
	addFragmentShader(Shader::loadShaderSource("shaders/forward-spot.fs"));
	compileShader();

	setAttribLocation(0, "position");
	setAttribLocation(1, "normal");
	setAttribLocation(2, "tangent");
	setAttribLocation(3, "texCoord");

	addUniform("model");
	addUniform("MVP");
	addUniform("eyePos");
	addUniform("spotLight.point.base.color");
	addUniform("spotLight.point.base.intensity");
	addUniform("spotLight.point.atten.constant");
	addUniform("spotLight.point.atten.linear");
	addUniform("spotLight.point.atten.exponent");
	addUniform("spotLight.point.position");
	addUniform("spotLight.direction");
	addUniform("spotLight.cutoff");
	addUniform("specularIntensity");
	addUniform("specularPower");
	addUniform("colorMap");
	addUniform("normalMap");

	spotLight.point.base.color = Vector3f(1, 1, 1);
	spotLight.point.base.intensity = 300.0f;
	spotLight.point.atten.constant = 0;
	spotLight.point.atten.linear = 0;
	spotLight.point.atten.exponent = 0.01f;
	spotLight.point.position = Vector3f(248, 288, -272);
	spotLight.direction = Vector3f(0, 0, -1);
	spotLight.cutoff = 0.95f;
}

void ForwardSpot::updateUniforms(Matrix4f &worldMatrix, Matrix4f &projectedMatrix, Material *material)
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
	glUniform3f(*uniforms->get("spotLight.point.base.color"), spotLight.point.base.color.getX(), spotLight.point.base.color.getY(), spotLight.point.base.color.getZ());
	glUniform1f(*uniforms->get("spotLight.point.base.intensity"), spotLight.point.base.intensity);
	glUniform1f(*uniforms->get("spotLight.point.atten.constant"), spotLight.point.atten.constant);
	glUniform1f(*uniforms->get("spotLight.point.atten.linear"), spotLight.point.atten.linear);
	glUniform1f(*uniforms->get("spotLight.point.atten.exponent"), spotLight.point.atten.exponent);
	glUniform3f(*uniforms->get("spotLight.point.position"), pos.getX(), pos.getY(), pos.getZ());
	Vector3f dir = Transform::getCamera()->getForward();
	glUniform3f(*uniforms->get("spotLight.direction"), dir.getX(), dir.getY(), dir.getZ());
	glUniform1f(*uniforms->get("spotLight.cutoff"), spotLight.cutoff);
	glUniform1f(*uniforms->get("specularIntensity"), material->getSpecularIntensity());
	glUniform1f(*uniforms->get("specularPower"), material->getSpecularPower());
}
