#include "ForwardDirectional.h"
#include "GLExtFunc.h"
#include "Transform.h"
#include <math.h>
#include "Time.h"

ForwardDirectional::ForwardDirectional()
{
	addVertexShader(Shader::loadShaderSource("shaders/forward-directional.vs"));
	addFragmentShader(Shader::loadShaderSource("shaders/forward-directional.fs"));
	compileShader();

	setAttribLocation(0, "position");
	setAttribLocation(1, "normal");
	setAttribLocation(2, "tangent");
	setAttribLocation(3, "texCoord");

	addUniform("model");
	addUniform("MVP");
	addUniform("eyePos");
	addUniform("directionalLight.base.color");
	addUniform("directionalLight.base.intensity");
	addUniform("directionalLight.dir");
	addUniform("specularIntensity");
	addUniform("specularPower");
	addUniform("colorMap");
	addUniform("normalMap");

	directionalLight.base.color = Vector3f(1, 1, 1);
	directionalLight.base.intensity = 0.2f;
	directionalLight.direction = Vector3f(1, 1, 1).normalized(); // en sol som er rett OVER
}

void ForwardDirectional::updateUniforms(Matrix4f &worldMatrix, Matrix4f &projectedMatrix, Material *material)
{
	float radPerSec = 2 * 3.14f / 4; // 1 minute per round

	//directionalLight.direction = Vector3f(sinf(radPerSec*(float)Time::getTime()), 0.324f, cosf(radPerSec*(float)Time::getTime())).normalized(); // en sol som er rett OVER
	//directionalLight.direction = Vector3f(sinf(radPerSec*(float)Time::getTime()), cosf(radPerSec*(float)Time::getTime()), 0).normalized(); // en sol som er rett OVER

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
	glUniform3f(*uniforms->get("directionalLight.base.color"), directionalLight.base.color.getX(), directionalLight.base.color.getY(), directionalLight.base.color.getZ());
	glUniform1f(*uniforms->get("directionalLight.base.intensity"), directionalLight.base.intensity);
	glUniform3f(*uniforms->get("directionalLight.dir"), directionalLight.direction.getX(), directionalLight.direction.getY(), directionalLight.direction.getZ());
	glUniform1f(*uniforms->get("specularIntensity"), material->getSpecularIntensity());
	glUniform1f(*uniforms->get("specularPower"), material->getSpecularPower());
}
