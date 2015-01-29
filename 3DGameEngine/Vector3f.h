#ifndef VECTOR3F_H
#define VECTOR3F_H

//#include "Matrix4f.h"

class Vector3f
{
public:
	Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	float length();
	float dot(Vector3f &v);

	Vector3f lerp(Vector3f &v, float alpha);
	Vector3f cross(Vector3f &v);
	Vector3f rotate(float angle, Vector3f &axis);
	Vector3f normalized();

	void operator+=(Vector3f &r);
	void operator-=(Vector3f &r);
	void operator*=(Vector3f &r);
	void operator/=(Vector3f &r);
	Vector3f operator+(Vector3f &r);
	Vector3f operator-();
	Vector3f operator-(Vector3f &r);
	Vector3f operator*(float f);
	Vector3f operator*(Vector3f &v);
	Vector3f operator/(float f);
	Vector3f operator/(Vector3f &v);

	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
	float getZ();
	void setZ(float z);

private:
	float x, y, z;
};

#endif