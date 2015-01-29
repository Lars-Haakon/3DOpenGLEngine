#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Vector3f.h"
#include "Vector2f.h"

struct Plane
{
	Vector3f normal;
	float distance;
};

struct Vertex
{
	Vector3f pos;
	Vector3f normal;
	Vector3f tangent;
	Vector2f texCoord;
};

struct Face
{
	Vector3f p0;
	Vector3f p1;
	Vector3f p2;
	Vector3f normal;
};

struct ray
{
	Vector3f point;
	Vector3f direction;
};

#endif