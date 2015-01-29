#ifndef VECTOR2F_H
#define VECTOR2F_H

class Vector2f
{
public:
	Vector2f(float x = 0.0f, float y = 0.0f);

	Vector2f operator+(Vector2f &r);
	Vector2f operator-(Vector2f &r);
	Vector2f operator*(Vector2f &v);
	Vector2f operator*(float amt);
	Vector2f operator/(Vector2f &v);
	Vector2f operator/(float amt);

	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
private:
	float x, y;
};

#endif