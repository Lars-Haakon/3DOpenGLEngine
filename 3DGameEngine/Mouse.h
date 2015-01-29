#ifndef MOUSE_H
#define MOUSE_H

#include "Geometry.h"
#include "Window.h"

class Mouse
{
public:
	static const char MOUSE_LEFT;
	static const char MOUSE_RIGHT;
	static const char MOUSE_MIDDLE;
	static const char MOUSE_X1;
	static const char MOUSE_X2;

	static void update();
	static void centerMousePos();

	static bool getMouseButton(int mouseButtonCode);
	static void setMouseButton(int mouseButtonCode, bool state);
	static bool getMouseButtonDown(int mouseButtonCode);
	static bool getMouseButtonUp(int mouseButtonCode);

	static Vector3f getMousepickRay();

	static void setWindow(Window *window);
	static Vector2f getMousePos();
	static void setMousePos(Vector2f pos);
	static Vector2f getDelta();
	static void setCursor(bool state);

private:
	static Window *window; // Window associated with mouse input

	static bool lastMouseButtonStates[];
	static bool mouseButtonStates[];

	static Vector2f mousePos;
};

#endif