#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec2 texCoord;

varying vec3 worldPos0;
varying vec2 texCoord0;
varying mat3 tbnMatrix;

uniform mat4 model;
uniform mat4 MVP;

void main()
{
	worldPos0 = (model * vec4(position, 1.0)).xyz;
	vec3 n = normalize((model * vec4(normal, 0.0)).xyz);
	texCoord0 = texCoord;
	vec3 t = normalize((model * vec4(tangent, 0.0)).xyz);
	
	t = normalize(t - n * dot(n, t));
	vec3 biTangent = cross(n, t);
	
	tbnMatrix = mat3(t, biTangent, n);
	
	gl_Position = MVP * vec4(position, 1.0);
}