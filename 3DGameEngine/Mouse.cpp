#include "Mouse.h"
#include "Console.h"
#include "Transform.h"

const char Mouse::MOUSE_LEFT = 0x0;
const char Mouse::MOUSE_MIDDLE = 0x1;
const char Mouse::MOUSE_RIGHT = 0x2;
const char Mouse::MOUSE_X1 = 0x3;
const char Mouse::MOUSE_X2 = 0x4;

Window *Mouse::window;

bool Mouse::lastMouseButtonStates[5];
bool Mouse::mouseButtonStates[5];

Vector2f Mouse::mousePos;

void Mouse::update()
{
	for (int i = 0; i < 5; i++)
		lastMouseButtonStates[i] = mouseButtonStates[i];
}

void Mouse::centerMousePos()
{
	SetCursorPos((int)(window->getScreenPos().getX() + (window->getScreenWidth() / 2.0f)), (int)(window->getScreenPos().getY() + (window->getScreenHeight() / 2.0f)));
	Mouse::mousePos  = Vector2f((float)(window->getScreenWidth() / 2.0f), (float)(window->getScreenHeight() / 2.0f));
}

bool Mouse::getMouseButton(int mouseButtonCode)
{
	return mouseButtonStates[mouseButtonCode];
}

void Mouse::setMouseButton(int mouseButtonCode, bool state)
{
	mouseButtonStates[mouseButtonCode] = state;
}

bool Mouse::getMouseButtonDown(int mouseButtonCode)
{
	return mouseButtonStates[mouseButtonCode] && !lastMouseButtonStates[mouseButtonCode];
}

bool Mouse::getMouseButtonUp(int mouseButtonCode)
{
	return !mouseButtonStates[mouseButtonCode] && lastMouseButtonStates[mouseButtonCode];
}

Vector3f Mouse::getMousepickRay()
{
	float x = (2.0f * mousePos.getX()) / window->getScreenWidth() - 1.0f;
	float y = 1.0f - (2.0f * mousePos.getY()) / window->getScreenHeight();

	Vector3f ray_clip = Vector3f(x, y, -1);
	Vector3f ray_eye = Transform::getProjectionMatrix().inverted() * ray_clip;
	ray_eye.setZ(-1);

	Matrix4f cameraRotation;
	Matrix4f cameraTranslation;
	cameraRotation.initCameraTransform(Transform::getCamera()->getForward(), Transform::getCamera()->getUp());
	cameraTranslation.initTranslationTransform(-Transform::getCamera()->getPos().getX(), -Transform::getCamera()->getPos().getY(), -Transform::getCamera()->getPos().getZ());

	Matrix4f viewMatrix = cameraRotation * cameraTranslation;
	Vector3f ray_wor = viewMatrix * ray_eye;

	return ray_wor.normalized();
}

void Mouse::setWindow(Window *window)
{
	Mouse::window = window;
}

Vector2f Mouse::getMousePos()
{
	return mousePos;
}

void Mouse::setMousePos(Vector2f pos)
{
	mousePos = pos;
}

Vector2f Mouse::getDelta()
{
	Vector2f centerPosition = Vector2f((float)(window->getScreenWidth() / 2.0f), (float)(window->getScreenHeight() / 2.0f));

	return Mouse::mousePos - centerPosition;
}

void Mouse::setCursor(bool state)
{
	ShowCursor(state);
}