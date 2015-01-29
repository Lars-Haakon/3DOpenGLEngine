#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4f.h"
#include "Camera.h"

class Transform
{
public:
	static Camera *getCamera();
	static void setCamera(Camera *camera);
	static void setProjectionMatrix(float fov, float aspect, float zNear, float zFar);
	static Matrix4f getProjectionMatrix();

	Transform(Vector3f pos = Vector3f(0.0f, 0.0f, 0.0f), Quaternion orientation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f), Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));
	~Transform();

	Matrix4f getTransformation();
	Matrix4f getProjectedTransformation();

	Vector3f &getPos();
	Quaternion &getOrientation();
	Vector3f &getScale();

	void setPos(Vector3f &pos);
	void setOrientation(Quaternion &orientation);
	void setScale(Vector3f &scale);
protected:
private:
	static Camera *m_camera;
	static Matrix4f projectionMatrix;

	/*static float m_fov;
	static float m_aspect;
	static float m_zNear;
	static float m_zFar;*/

	Vector3f m_pos;
	Quaternion m_orientation;
	Vector3f m_scale;
};

#endif