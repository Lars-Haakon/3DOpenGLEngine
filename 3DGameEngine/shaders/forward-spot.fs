#version 120

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	vec3 position;
};

struct SpotLight
{
	PointLight point;
	vec3 direction;
	float cutoff;
};

varying vec3 worldPos0;
varying vec2 texCoord0;
varying mat3 tbnMatrix;

uniform vec3 eyePos;

uniform float specularIntensity;
uniform float specularPower;

uniform SpotLight spotLight;

uniform sampler2D colorMap;
uniform sampler2D normalMap;

vec4 calcLight(BaseLight base, vec3 lightDirection, vec3 normal)
{
	vec4 diffuseColor = vec4(0, 0, 0, 1);
	vec4 specularColor = vec4(0, 0, 0, 1);

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

vec4 calcPointLight(PointLight light, vec3 normal)
{
	vec3 lightDirection = worldPos0 - light.position;
	float distanceToPoint = length(lightDirection);
	float attenuation = 1/(light.atten.constant +
						light.atten.linear * distanceToPoint +
						light.atten.exponent * distanceToPoint * distanceToPoint +
						0.0001);
	
	if(attenuation < 0.00001)
		return vec4(0, 0, 0, 1);
		
	lightDirection = normalize(lightDirection);
	
	vec4 color = calcLight(light.base, lightDirection, normal);
						
	return color * attenuation;
}

vec4 calcSpotLight(SpotLight light, vec3 normal)
{
	vec3 lightDirection = normalize(worldPos0 - light.point.position);
	float spotFactor = dot(lightDirection, light.direction);
	
	vec4 color = vec4(0, 0, 0, 1);
	
	if(spotFactor > light.cutoff)
	{
		color = calcPointLight(light.point, normal) * 
				(1.0 - (1.0 - spotFactor)/(1.0 - light.cutoff));
	}
	
	return color;
}

void main()
{
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoord0).xyz - 1));
    gl_FragColor = texture2D(colorMap, texCoord0) * calcSpotLight(spotLight, normal);
}