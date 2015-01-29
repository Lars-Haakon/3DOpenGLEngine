#include "Material.h"

Material::Material(Texture *colorMap, Texture *normalMap, float specularIntensity, float specularPower)
{
	this->colorMap = colorMap;
	this->normalMap = normalMap;
	this->specularIntensity = specularIntensity;
	this->specularPower = specularPower;
}

Material::~Material()
{
	delete colorMap;
	delete normalMap;
}

Texture *Material::getColorMap()
{
	return colorMap;
}

Texture *Material::getNormalMap()
{
	return normalMap;
}

float Material::getSpecularIntensity()
{
	return specularIntensity;
}

float Material::getSpecularPower()
{
	return specularPower;
}