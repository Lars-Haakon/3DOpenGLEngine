#include "ForwardAmbient.h"
#include "glExtFunc.h"

ForwardAmbient::ForwardAmbient()
{
	addVertexShader(Shader::loadShaderSource("shaders/forward-ambient.vs"));
	addFragmentShader(Shader::loadShaderSource("shaders/forward-ambient.fs"));

	setAttribLocation(0, "position");
	setAttribLocation(3, "texCoord");

	compileShader();
	addUniform("MVP");
	addUniform("ambientIntensity");
	addUniform("colorMap");
}

void ForwardAmbient::updateUniforms(Matrix4f &worldMatrix, Matrix4f &projectedMatrix, Material *material)
{
	glActiveTexture(GL_TEXTURE0);
	material->getColorMap()->bind();
	glUniform1i(*uniforms->get("colorMap"), 0);

	glUniformMatrix4fv(*uniforms->get("MVP"), 1, GL_TRUE, &(projectedMatrix[0][0]));
	glUniform3f(*uniforms->get("ambientIntensity"), 0.5f, 0.5f, 0.5f);
}
