#version 120

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 dir;
};

varying vec3 worldPos0;
varying vec2 texCoord0;
varying mat3 tbnMatrix;

uniform vec3 eyePos;
uniform DirectionalLight directionalLight;

uniform float specularIntensity;
uniform float specularPower;

uniform sampler2D colorMap;
uniform sampler2D normalMap;

vec4 calcLight(BaseLight base, vec3 lightDirection, vec3 normal)
{
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	float diffuseFactor = dot(normal, -lightDirection);
	
	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(eyePos - worldPos0);
		vec3 reflectDirection = normalize(reflect(lightDirection, normal));
		float specularFactor = pow(max(0.0, dot(directionToEye, reflectDirection)), specularPower);
		
		if(specularFactor > 0)
			specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
	}
	
	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal)
{	
	return calcLight(light.base, -light.dir, normal);
}

void main()
{
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoord0).xyz - 1));
    gl_FragColor = texture2D(colorMap, texCoord0) * calcDirectionalLight(directionalLight, normal);
}