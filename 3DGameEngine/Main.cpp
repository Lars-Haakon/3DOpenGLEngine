#include <Windows.h>
#include "Console.h"
#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE	hInstance,			// application Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	Console::initConsole();
	Window::setInstance(hInstance);

	Window *gameWindow = new GameWindow("GameWindow", "Tittel", 1280, 720, false);
	gameWindow->show();

	Console::releaseConsole();

	return 0;
}