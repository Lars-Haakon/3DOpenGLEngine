#include "Vector2f.h"

Vector2f::Vector2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2f Vector2f::operator*(Vector2f &v)
{
	return Vector2f(x * v.getX(), y * v.getY());
}

Vector2f Vector2f::operator*(float amt)
{
	return Vector2f(x * amt, y * amt);
}

Vector2f Vector2f::operator/(Vector2f &v)
{
	return Vector2f(x / v.getX(), y / v.getY());
}

Vector2f Vector2f::operator/(float amt)
{
	return Vector2f(x / amt, y / amt);
}

Vector2f Vector2f::operator+(Vector2f &v)
{
	return Vector2f(x + v.x, y + v.y);
}

Vector2f Vector2f::operator-(Vector2f &v)
{
	return Vector2f(x - v.getX(), y - v.getY());
}

float Vector2f::getX()
{
	return x;
}

void Vector2f::setX(float x)
{
	this->x = x;
}

float Vector2f::getY()
{
	return y;
}

void Vector2f::setY(float y)
{
	this->y = y;
}