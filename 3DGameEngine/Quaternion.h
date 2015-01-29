#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3f.h"

class Quaternion
{
public:
	Quaternion(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f);

	float length();
	void normalize();
	Quaternion normalized();

	Quaternion conjugate();

	static Quaternion constructFromEulerAngles(float angle, Vector3f &axis);

	void operator+=(Quaternion &q);
	Quaternion operator+(Quaternion &q);
	Quaternion operator-(Quaternion &q);
	Quaternion operator*(Quaternion &q);
	Quaternion operator*(float amt);
	Quaternion operator/(float amt);
	Quaternion lerp(Quaternion &q, float alpha);
	Quaternion slerp(Quaternion &q, float alpha);

	float getX();
	float getY();
	float getZ();
	float getW();
private:
	float x, y, z, w;
};

#endif