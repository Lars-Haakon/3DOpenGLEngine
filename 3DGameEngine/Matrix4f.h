#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.h"

class Matrix4f
{
public:
	void initIdentity();
	void initScaleTransform(float scaleX, float scaleY, float scaleZ);
	void initRotateTransform(float rotateX, float rotateY, float rotateZ);
	void initRotateTransform(float x, float y, float z, float w);
	void initTranslationTransform(float x, float y, float z);
	void initCameraTransform(Vector3f &target, Vector3f &up);
	void initPersProjTransform(float fov, float aspect, float zNear, float zFar);

	void initCubeoidInertiaTensor(float width, float height, float depth, float mass);
	void initCubeoidInertiaTensor(Vector3f &halfSize, float mass);

	Matrix4f operator+(Matrix4f &right);
	Matrix4f operator*(float f);
	Vector3f operator*(Vector3f &v);
	Matrix4f operator*(Matrix4f &right);
	Matrix4f transposed();
	float determinant();
	Matrix4f inverted();
	float *operator[](int index);
private:
	float m[4][4];
};

#endif