#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Window.h"
#include <Windows.h>

class GameWindow : public Window
{
public:
	GameWindow(const char *className, const char *title, int width, int height, bool fullscreen);

	virtual LRESULT windowEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:

};

#endif