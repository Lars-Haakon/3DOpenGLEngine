#ifndef COREENGINE_H
#define COREENGINE_H

#include "Window.h"
#include "TestGame.h"

#include <Windows.h>

class CoreEngine
{
public:
	double FPS;

	CoreEngine(Window *window);
	~CoreEngine();

	void run(HDC &hdc);
private:
	void glInit();
	void setProjection(int w, int h);

	TestGame *game;
};

#endif