#include "Quaternion.h"

#include <math.h>

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

float Quaternion::length()
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

void Quaternion::normalize()
{
	float quaternionLength = length();

	x /= quaternionLength;
	y /= quaternionLength;
	z /= quaternionLength;
	w /= quaternionLength;
}

Quaternion Quaternion::normalized()
{
	float quaternionLength = length();

	return Quaternion(x / quaternionLength, y / quaternionLength, z / quaternionLength, w / quaternionLength);
}

Quaternion Quaternion::conjugate()
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::constructFromEulerAngles(float angle, Vector3f &axis)
{
	float sinHalfAngle = sin(angle * 3.14f / 360.0f);
	float cosHalfAngle = cos(angle * 3.14f / 360.0f);

	float _x = axis.getX() * sinHalfAngle;
	float _y = axis.getY() * sinHalfAngle;
	float _z = axis.getZ() * sinHalfAngle;
	float _w = cosHalfAngle;

	return Quaternion(_x, _y, _z, _w);
}

void Quaternion::operator+=(Quaternion &q)
{
	x += q.getX();
	y += q.getY();
	z += q.getZ();
	w += q.getW();
}

Quaternion Quaternion::operator+(Quaternion &q)
{
	return Quaternion(x + q.getX(), y + q.getY(), z + q.getZ(), w + q.getW());
}

Quaternion Quaternion::operator-(Quaternion &q)
{
	return Quaternion(x - q.getX(), y - q.getY(), z - q.getZ(), w - q.getW());
}

Quaternion Quaternion::operator*(Quaternion &r)
{
	const float _w = (w * r.getW()) - (x * r.getX()) - (y * r.getY()) - (z * r.getZ());
	const float _x = (w * r.getX()) + (x * r.getW()) + (y * r.getZ()) - (z * r.getY());
	const float _y = (w * r.getY()) - (x * r.getZ()) + (y * r.getW()) + (z * r.getX());
	const float _z = (w * r.getZ()) + (x * r.getY()) - (y * r.getX()) + (z * r.getW());

	return Quaternion(_x, _y, _z, _w);
}

Quaternion Quaternion::operator*(float amt)
{
	return Quaternion(x * amt, y * amt, z * amt, w * amt);
}

Quaternion Quaternion::operator/(float amt)
{
	return Quaternion(x / amt, y / amt, z / amt, w / amt);
}

//! linear quaternion interpolation
Quaternion Quaternion::lerp(Quaternion &q, float alpha)
{
	return (*this * (1 - alpha) + q * alpha).normalized();
}

//! spherical linear interpolation
Quaternion Quaternion::slerp(Quaternion &q, float alpha)
{
	if (alpha <= 0.0)
		return *this;
	if (alpha >= 1.0)
		return q;

	Quaternion q2 = Quaternion(q.getX(), q.getY(), q.getZ(), q.getW());
	float dot = x * q.getX() + y * q.getY() + z * q.getZ() + w * q.getW();

	/*	dot = cos(theta)
	if (dot < 0), *this and q are more than 90 degrees apart,
	so we can invert one to reduce spinning	*/

	if (dot < 0)
	{
		dot = -dot;
		q2 = Quaternion(-q.getX(), -q.getY(), -q.getZ(), -q.getW());
	}

	if (dot < 0.95f)
	{
		float theta = acosf(dot);

		return ((*this*sinf(theta*(1 - alpha)) + q2*sinf(theta*alpha)) / sinf(theta)).normalized();
	}
	else // if the angle is small, use linear interpolation	
		return lerp(q2, alpha);
}

float Quaternion::getX()
{
	return x;
}

float Quaternion::getY()
{
	return y;
}
float Quaternion::getZ()
{
	return z;
}
float Quaternion::getW()
{
	return w;
}