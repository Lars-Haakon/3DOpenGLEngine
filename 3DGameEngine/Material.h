#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

class Material
{
public:
	Material(Texture *colorMap, Texture *normalMap, float specularIntensity, float specularPower);
	~Material();

	Texture *getColorMap();
	Texture *getNormalMap();

	float getSpecularIntensity();
	float getSpecularPower();
private:
	Texture *colorMap;
	Texture *normalMap;

	float specularIntensity;
	float specularPower;
	float restitution;
};

#endif