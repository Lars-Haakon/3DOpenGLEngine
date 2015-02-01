#include "matrix4f.h"
#include <math.h>

void Matrix4f::initIdentity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::initScaleTransform(float scaleX, float scaleY, float scaleZ)
{
	m[0][0] = scaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = scaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaleZ; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix4f::initRotateTransform(float rotateX, float rotateY, float rotateZ)
{
	Matrix4f rx, ry, rz;

	const float x = rotateX * 3.14159265359f / 180.0f;
	const float y = rotateY * 3.14159265359f / 180.0f;
	const float z = rotateZ * 3.14159265359f / 180.0f;

	rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f;		rx.m[0][2] = 0.0f;		rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x);	rx.m[1][2] = -sinf(x);	rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x);	rx.m[2][2] = cosf(x);	rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f;		rx.m[3][2] = 0.0f;		rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(y);	ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y);	ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f;		ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f;		ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(y);	ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y);	ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f;		ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f;		ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(z);	rz.m[0][1] = -sinf(z);	rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(z);	rz.m[1][1] = cosf(z);	rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f;		rz.m[2][1] = 0.0f;		rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f;		rz.m[3][1] = 0.0f;		rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4f::initRotateTransform(float x, float y, float z, float w)
{
	m[0][0] = 1 - 2.0f * (y * y + z * z);	m[0][1] = 2.0f * (x * y + z * w);		m[0][2] = 2.0f * (x * z - y * w);		m[0][3] = 0.0f;
	m[1][0] = 2.0f * (x * y - z * w);		m[1][1] = 1 - 2.0f * (x * x + z * z);	m[1][2] = 2.0f * (y * z + x * w);		m[1][3] = 0.0f;
	m[2][0] = 2.0f * (x * z + y * w);		m[2][1] = 2.0f * (y * z - x * w);		m[2][2] = 1 - 2.0f * (x * x + y * y);	m[2][3] = 0.0f;
	m[3][0] = 0.0f;							m[3][1] = 0.0f;							m[3][2] = 0.0f;							m[3][3] = 1.0f;
}

void Matrix4f::initTranslationTransform(float x, float y, float z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::initCameraTransform(Vector3f& forward, Vector3f& up)
{
	Vector3f n = forward.normalized(); // forward
	Vector3f u = up.normalized(); // up
	Vector3f v = n.cross(u); // right

	m[0][0] = v.getX();		m[0][1] = v.getY();		m[0][2] = v.getZ();		m[0][3] = 0.0f;
	m[1][0] = u.getX();		m[1][1] = u.getY();		m[1][2] = u.getZ();		m[1][3] = 0.0f;
	m[2][0] = -n.getX();	m[2][1] = -n.getY();	m[2][2] = -n.getZ();	m[2][3] = 0.0f;
	m[3][0] = 0.0f;			m[3][1] = 0.0f;			m[3][2] = 0.0f;			m[3][3] = 1.0f;
}

void Matrix4f::initPersProjTransform(float fov, float aspect, float zNear, float zFar)
{
	float flRight = zNear * tanf(fov * 3.14159265359f / 360.0f);
	float flLeft = -flRight;
	float flTop = flRight / aspect;
	float flBottom = flLeft / aspect;

	m[0][0] = 2 * zNear / (flRight - flLeft);	m[0][1] = 0.0f;								m[0][2] = (flRight + flLeft) / (flRight - flLeft);	m[0][3] = 0.0f;
	m[1][0] = 0.0f;								m[1][1] = 2 * zNear / (flTop - flBottom);	m[1][2] = (flTop + flBottom) / (flTop - flBottom);	m[1][3] = 0.0f;
	m[2][0] = 0.0f;								m[2][1] = 0.0f;								m[2][2] = (-zFar - zNear) / (zFar - zNear);			m[2][3] = -2 * zFar * zNear / (zFar - zNear);
	m[3][0] = 0.0f;								m[3][1] = 0.0f;								m[3][2] = -1.0f;									m[3][3] = 0.0f;
}

void Matrix4f::initCubeoidInertiaTensor(float w, float h, float d, float mass)
{
	m[0][0] = mass * (h * h + d * d) / 12;	m[0][1] = 0.0f;							m[0][2] = 0;							m[0][3] = 0.0f;
	m[1][0] = 0.0f;							m[1][1] = mass * (w * w + d * d) / 12;	m[1][2] = 0;							m[1][3] = 0.0f;
	m[2][0] = 0.0f;							m[2][1] = 0.0f;							m[2][2] = mass * (w * w + h * h) / 12;	m[2][3] = 0.0f;
	m[3][0] = 0.0f;							m[3][1] = 0.0f;							m[3][2] = 0.0f;							m[3][3] = 1.0f;
}

void Matrix4f::initCubeoidInertiaTensor(Vector3f &halfSize, float mass)
{
	Vector3f squared = halfSize * halfSize;

	m[0][0] = mass*(squared.getY()+squared.getZ())/3;	m[0][1] = 0.0f;										m[0][2] = 0;										m[0][3] = 0.0f;
	m[1][0] = 0.0f;										m[1][1] = mass*(squared.getX()+squared.getZ())/3;	m[1][2] = 0;										m[1][3] = 0.0f;
	m[2][0] = 0.0f;										m[2][1] = 0.0f;										m[2][2] = mass*(squared.getX()*squared.getY())/3;	m[2][3] = 0.0f;
	m[3][0] = 0.0f;										m[3][1] = 0.0f;										m[3][2] = 0.0f;										m[3][3] = 1.0f;
}

Matrix4f Matrix4f::operator+(Matrix4f &right)
{
	Matrix4f ret;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret[i][j] = m[i][j] + right[i][j];
		}
	}

	return ret;
}

Matrix4f Matrix4f::operator*(float f)
{
	Matrix4f ret;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret[i][j] = m[i][j] * f;
		}
	}

	return ret;
}

Vector3f Matrix4f::operator*(Vector3f &v)
{
	float x = m[0][0] * v.getX() + m[0][1] * v.getY() + m[0][2] * v.getZ() + m[0][3];
	float y = m[1][0] * v.getX() + m[1][1] * v.getY() + m[1][2] * v.getZ() + m[1][3];
	float z = m[2][0] * v.getX() + m[2][1] * v.getY() + m[2][2] * v.getZ() + m[2][3];

	return Vector3f(x, y, z);
}

Matrix4f Matrix4f::operator*(Matrix4f& right)
{
	Matrix4f ret;
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			ret.m[i][j] =	m[i][0] * right.m[0][j] +
							m[i][1] * right.m[1][j] +
							m[i][2] * right.m[2][j] +
							m[i][3] * right.m[3][j];
		}
	}
	return ret;
}

Matrix4f Matrix4f::transposed()
{
	Matrix4f ret;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret[i][j] = m[j][i];
		}
	}

	return ret;
}

float Matrix4f::determinant()
{
	float r0 = m[1][1] * m[2][2] * m[3][3] -
		m[1][1] * m[2][3] * m[3][2] -
		m[2][1] * m[1][2] * m[3][3] +
		m[2][1] * m[1][3] * m[3][2] +
		m[3][1] * m[1][2] * m[2][3] -
		m[3][1] * m[1][3] * m[2][2];

	float r1 = -m[1][0] * m[2][2] * m[3][3] +
		m[1][0] * m[2][3] * m[3][2] +
		m[2][0] * m[1][2] * m[3][3] -
		m[2][0] * m[1][3] * m[3][2] -
		m[3][0] * m[1][2] * m[2][3] +
		m[3][0] * m[1][3] * m[2][2];

	float r2 = m[1][0] * m[2][1] * m[3][3] -
		m[1][0] * m[2][3] * m[3][1] -
		m[2][0] * m[1][1] * m[3][3] +
		m[2][0] * m[1][3] * m[3][1] +
		m[3][0] * m[1][1] * m[2][3] -
		m[3][0] * m[1][3] * m[2][1];

	float r3 = -m[1][0] * m[2][1] * m[3][2] +
		m[1][0] * m[2][2] * m[3][1] +
		m[2][0] * m[1][1] * m[3][2] -
		m[2][0] * m[1][2] * m[3][1] -
		m[3][0] * m[1][1] * m[2][2] +
		m[3][0] * m[1][2] * m[2][1];

	return m[0][0] * r0 + m[0][1] * r1 + m[0][2] * r2 + m[0][3] * r3;
}

Matrix4f Matrix4f::inverted()
{
	Matrix4f ret;
	ret[0][0] = m[1][1] * m[2][2] * m[3][3] -
		m[1][1] * m[2][3] * m[3][2] -
		m[2][1] * m[1][2] * m[3][3] +
		m[2][1] * m[1][3] * m[3][2] +
		m[3][1] * m[1][2] * m[2][3] -
		m[3][1] * m[1][3] * m[2][2];

	ret[1][0] = -m[1][0] * m[2][2] * m[3][3] +
		m[1][0] * m[2][3] * m[3][2] +
		m[2][0] * m[1][2] * m[3][3] -
		m[2][0] * m[1][3] * m[3][2] -
		m[3][0] * m[1][2] * m[2][3] +
		m[3][0] * m[1][3] * m[2][2];

	ret[2][0] = m[1][0] * m[2][1] * m[3][3] -
		m[1][0] * m[2][3] * m[3][1] -
		m[2][0] * m[1][1] * m[3][3] +
		m[2][0] * m[1][3] * m[3][1] +
		m[3][0] * m[1][1] * m[2][3] -
		m[3][0] * m[1][3] * m[2][1];

	ret[3][0] = -m[1][0] * m[2][1] * m[3][2] +
		m[1][0] * m[2][2] * m[3][1] +
		m[2][0] * m[1][1] * m[3][2] -
		m[2][0] * m[1][2] * m[3][1] -
		m[3][0] * m[1][1] * m[2][2] +
		m[3][0] * m[1][2] * m[2][1];

	ret[0][1] = -m[0][1] * m[2][2] * m[3][3] +
		m[0][1] * m[2][3] * m[3][2] +
		m[2][1] * m[0][2] * m[3][3] -
		m[2][1] * m[0][3] * m[3][2] -
		m[3][1] * m[0][2] * m[2][3] +
		m[3][1] * m[0][3] * m[2][2];

	ret[1][1] = m[0][0] * m[2][2] * m[3][3] -
		m[0][0] * m[2][3] * m[3][2] -
		m[2][0] * m[0][2] * m[3][3] +
		m[2][0] * m[0][3] * m[3][2] +
		m[3][0] * m[0][2] * m[2][3] -
		m[3][0] * m[0][3] * m[2][2];

	ret[2][1] = -m[0][0] * m[2][1] * m[3][3] +
		m[0][0] * m[2][3] * m[3][1] +
		m[2][0] * m[0][1] * m[3][3] -
		m[2][0] * m[0][3] * m[3][1] -
		m[3][0] * m[0][1] * m[2][3] +
		m[3][0] * m[0][3] * m[2][1];

	ret[3][1] = m[0][0] * m[2][1] * m[3][2] -
		m[0][0] * m[2][2] * m[3][1] -
		m[2][0] * m[0][1] * m[3][2] +
		m[2][0] * m[0][2] * m[3][1] +
		m[3][0] * m[0][1] * m[2][2] -
		m[3][0] * m[0][2] * m[2][1];

	ret[0][2] = m[0][1] * m[1][2] * m[3][3] -
		m[0][1] * m[1][3] * m[3][2] -
		m[1][1] * m[0][2] * m[3][3] +
		m[1][1] * m[0][3] * m[3][2] +
		m[3][1] * m[0][2] * m[1][3] -
		m[3][1] * m[0][3] * m[1][2];

	ret[1][2] = -m[0][0] * m[1][2] * m[3][3] +
		m[0][0] * m[1][3] * m[3][2] +
		m[1][0] * m[0][2] * m[3][3] -
		m[1][0] * m[0][3] * m[3][2] -
		m[3][0] * m[0][2] * m[1][3] +
		m[3][0] * m[0][3] * m[1][2];

	ret[2][2] = m[0][0] * m[1][1] * m[3][3] -
		m[0][0] * m[1][3] * m[3][1] -
		m[1][0] * m[0][1] * m[3][3] +
		m[1][0] * m[0][3] * m[3][1] +
		m[3][0] * m[0][1] * m[1][3] -
		m[3][0] * m[0][3] * m[1][1];

	ret[3][2] = -m[0][0] * m[1][1] * m[3][2] +
		m[0][0] * m[1][2] * m[3][1] +
		m[1][0] * m[0][1] * m[3][2] -
		m[1][0] * m[0][2] * m[3][1] -
		m[3][0] * m[0][1] * m[1][2] +
		m[3][0] * m[0][2] * m[1][1];

	ret[0][3] = -m[0][1] * m[1][2] * m[2][3] +
		m[0][1] * m[1][3] * m[2][2] +
		m[1][1] * m[0][2] * m[2][3] -
		m[1][1] * m[0][3] * m[2][2] -
		m[2][1] * m[0][2] * m[1][3] +
		m[2][1] * m[0][3] * m[1][2];

	ret[1][3] = m[0][0] * m[1][2] * m[2][3] -
		m[0][0] * m[1][3] * m[2][2] -
		m[1][0] * m[0][2] * m[2][3] +
		m[1][0] * m[0][3] * m[2][2] +
		m[2][0] * m[0][2] * m[1][3] -
		m[2][0] * m[0][3] * m[1][2];

	ret[2][3] = -m[0][0] * m[1][1] * m[2][3] +
		m[0][0] * m[1][3] * m[2][1] +
		m[1][0] * m[0][1] * m[2][3] -
		m[1][0] * m[0][3] * m[2][1] -
		m[2][0] * m[0][1] * m[1][3] +
		m[2][0] * m[0][3] * m[1][1];

	ret[3][3] = m[0][0] * m[1][1] * m[2][2] -
		m[0][0] * m[1][2] * m[2][1] -
		m[1][0] * m[0][1] * m[2][2] +
		m[1][0] * m[0][2] * m[2][1] +
		m[2][0] * m[0][1] * m[1][2] -
		m[2][0] * m[0][2] * m[1][1];


	// TEST DETERMINENT?????????
	float det = m[0][0] * ret[0][0] + m[0][1] * ret[1][0] + m[0][2] * ret[2][0] + m[0][3] * ret[3][0];

	return ret * (1 / det);
}

float *Matrix4f::operator[](int index)
{
	return m[index];
}