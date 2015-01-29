#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3f.h"

class Camera
{
public:
	Camera(Vector3f pos = Vector3f(0.0f, 20.0f, 50.0f), Vector3f forward = Vector3f(0.0f, 0.0f, -1.0f), Vector3f up = Vector3f(0.0f, 1.0f, 0.0f));
	~Camera();

	void input();
	void move(Vector3f &direction, float amt);
	void rotateY(float angle);
	void rotateX(float angle);

	Vector3f &getPos();
	Vector3f &getForward();
	Vector3f &getUp();

	Vector3f getLeft();
	Vector3f getRight();
protected:
private:
	Vector3f m_pos;
	Vector3f m_forward;
	Vector3f m_up;

	Vector3f UP;
};

#endif