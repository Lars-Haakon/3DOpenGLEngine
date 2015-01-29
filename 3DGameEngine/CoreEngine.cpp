#include "CoreEngine.h"
#include "glExtFunc.h"
#include <ctime>
#include "Time.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Console.h"
#include "Transform.h"

CoreEngine::CoreEngine(Window *window)
{
	glExtFuncInit();
	glInit();

	FPS = 500.0; // MAX 1000, can go higher for benchmark

	srand((unsigned int)time(NULL)); // seed for randomness
	Mouse::setWindow(window);
	Transform::setProjectionMatrix(75.0f, window->getScreenWidth() / (float) window->getScreenHeight(), 0.1f, 10000.0f);
	game = new TestGame();
}

CoreEngine::~CoreEngine()
{
	delete game;
}

void CoreEngine::run(HDC &hdc)
{
	double frameTime = 1.0 / FPS;
	double lastTime = Time::startCounter();
	double accumulator = 0.0;

	int frames = 0;
	double frameCounter = 0.0;

	bool done = false;
	MSG msg;
	do
	{
		double newTime = Time::getSystemTime();
		double passedTime = newTime - lastTime;
		lastTime = newTime;

		if (passedTime > 0.10) // avoiding spiral of death
			passedTime = 0.10;

		accumulator += passedTime;
		frameCounter += passedTime;

		bool render = false;
		while (accumulator >= frameTime)
		{
			render = true;

			accumulator -= frameTime;
			Time::setDeltaTime(frameTime);
			Time::update();

			game->input();
			Mouse::update();
			Keyboard::update();

			game->update();

			if (frameCounter >= 1.0)
			{
				Console::printf("FPS: %d\n", frames);

				frames = 0;
				frameCounter = 0.0;
			}
		}

		if (render)
		{
			Time::setAlpha(accumulator / frameTime);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			game->render();
			SwapBuffers(hdc);

			frames++;
		}
		else
			Sleep(1);
	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_QUIT:
				done = true;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (!done);
}

void CoreEngine::glInit()
{
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_CULL_FACE); // dont draw backside of faces
	glEnable(GL_DEPTH_TEST); // enable depth
	glEnable(GL_DEPTH_CLAMP); // prevent seeing through meshes

	wglSwapIntervalEXT(0); // disable vSync

	Console::printf("%s\n", glGetString(GL_VERSION)); // print OpenGL version

	int textureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);
	Console::printf("Max Texture sixe %d\n", textureSize); // print Maximum texture size

	int maxTextureUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	Console::printf("Max texture units %d\n", maxTextureUnits); // print Maximum texture units
}