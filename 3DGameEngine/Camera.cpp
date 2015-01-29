#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Time.h"
#include "Console.h"

Camera::Camera(Vector3f pos, Vector3f forward, Vector3f up)
{
	m_pos = pos;
	m_forward = forward;
	m_up = up;

	UP = Vector3f(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{

}

bool mouseLocked = false;

void Camera::input()
{
	float sensitivity = 1.5f / 10.0f;
	float movAmt = (float)(500 * Time::getDeltaTime());

	if (Keyboard::getKeyDown(Keyboard::KEY_ESC))
	{
		Mouse::setCursor(true);
		mouseLocked = false;
	}
	if (Mouse::getMouseButtonDown(Mouse::MOUSE_LEFT))
	{
		Mouse::centerMousePos();
		Mouse::setCursor(false);
		mouseLocked = true;
	}

	if (mouseLocked)
	{
		Vector2f deltaPos = Mouse::getDelta();

		bool rotY = deltaPos.getX() != 0;
		bool rotX = deltaPos.getY() != 0;

		if (rotY)
			rotateY(-deltaPos.getX() * sensitivity);
		if (rotX)
			rotateX(deltaPos.getY() * sensitivity);

		if (rotY || rotX)
			Mouse::centerMousePos();
	}

	if (Keyboard::getKey(Keyboard::KEY_W))
		move(getForward(), movAmt);
	if (Keyboard::getKey(Keyboard::KEY_S))
		move(getForward(), -movAmt);
	if (Keyboard::getKey(Keyboard::KEY_A))
		move(getLeft(), movAmt);
	if (Keyboard::getKey(Keyboard::KEY_D))
		move(getRight(), movAmt);
}

void Camera::move(Vector3f& direction, float amt)
{
	m_pos += (direction * amt);
}

void Camera::rotateY(float angle)
{
	Vector3f hAxis = UP.cross(m_forward).normalized();

	m_forward = m_forward.rotate(angle, UP).normalized();
	m_up = m_forward.cross(hAxis).normalized();
}

void Camera::rotateX(float angle)
{
	Vector3f hAxis = UP.cross(m_forward).normalized();

	Vector3f tmpF = m_forward.rotate(angle, hAxis).normalized();
	Vector3f tmpU = tmpF.cross(hAxis).normalized();

	if (tmpU.getY() >= 0)
	{
		m_forward = tmpF;
		m_up = tmpU;
	}
}

Vector3f &Camera::getPos()
{
	return m_pos;
}

Vector3f& Camera::getForward()
{
	return m_forward;
}

Vector3f& Camera::getUp()
{
	return m_up;
}

Vector3f Camera::getLeft()
{
	return m_up.cross(m_forward).normalized();
}

Vector3f Camera::getRight()
{
	return m_forward.cross(m_up).normalized();
}