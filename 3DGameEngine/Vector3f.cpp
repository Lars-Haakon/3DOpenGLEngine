#include "Vector3f.h"
#include <math.h>
#include "Quaternion.h"

Vector3f::Vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3f::length()
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3f::dot(Vector3f &v)
{
	return x * v.getX() + y * v.getY() + z * v.getZ();
}

Vector3f Vector3f::lerp(Vector3f &v, float alpha)
{
	if (alpha <= 0.0)
		return *this;
	if (alpha >= 1.0)
		return v;

	return *this * (1 - alpha) + v * alpha;
}

Vector3f Vector3f::cross(Vector3f &v)
{
	return Vector3f(y * v.getZ() - z * v.getY(), z * v.getX() - x * v.getZ(), x * v.getY() - y * v.getX());
}

Vector3f Vector3f::rotate(float angle, Vector3f &axe)
{
	const float sinHalfAngle = sinf((angle / 2) * 3.14159265359f / 180.0f);
	const float cosHalfAngle = cosf((angle / 2) * 3.14159265359f / 180.0f);

	const float Rx = axe.getX() * sinHalfAngle;
	const float Ry = axe.getY() * sinHalfAngle;
	const float Rz = axe.getZ() * sinHalfAngle;
	const float Rw = cosHalfAngle;

	Quaternion rotationQ(Rx, Ry, Rz, Rw);

	Quaternion conjugateQ = rotationQ.conjugate();
	//  ConjugateQ.Normalize();
	Quaternion w = rotationQ * Quaternion(x, y, z, 0.0f) * conjugateQ;

	return Vector3f(w.getX(), w.getY(), w.getZ());
}

Vector3f Vector3f::normalized()
{
	float vectorLength = length();

	return Vector3f(x /= vectorLength, y /= vectorLength, z /= vectorLength);
}

Vector3f Vector3f::operator*(Vector3f &v)
{
	return Vector3f(x * v.getX(), y * v.getY(), z * v.getZ());
}

Vector3f Vector3f::operator*(float f)
{
	return Vector3f(x * f, y * f, z * f);
}

Vector3f Vector3f::operator/(Vector3f &v)
{
	return Vector3f(x / v.getX(), y / v.getY(), z / v.getZ());
}

Vector3f Vector3f::operator/(float f)
{
	return Vector3f(x / f, y / f, z / f);
}

void Vector3f::operator+=(Vector3f &r)
{
	x += r.getX();
	y += r.getY();
	z += r.getZ();
}

void Vector3f::operator-=(Vector3f &r)
{
	x -= r.getX();
	y -= r.getY();
	z -= r.getZ();
}

void Vector3f::operator*=(Vector3f &r)
{
	x *= r.getX();
	y *= r.getY();
	z *= r.getZ();
}

void Vector3f::operator/=(Vector3f &r)
{
	x /= r.getX();
	y /= r.getY();
	z /= r.getZ();
}

Vector3f Vector3f::operator+(Vector3f &r)
{
	return Vector3f(x + r.getX(), y + r.getY(), z + r.getZ());
}

Vector3f Vector3f::operator-()
{
	return Vector3f(-x, -y, -z);
}

Vector3f Vector3f::operator-(Vector3f &r)
{
	return Vector3f(x - r.getX(), y - r.getY(), z - r.getZ());
}

float Vector3f::getX()
{
	return x;
}

void Vector3f::setX(float x)
{
	this->x = x;
}

float Vector3f::getY()
{
	return y;
}

void Vector3f::setY(float y)
{
	this->y = y;
}

float Vector3f::getZ()
{
	return z;
}

void Vector3f::setZ(float z)
{
	this->z = z;
}