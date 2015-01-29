#include "GameWindow.h"

#include "CoreEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Vector2f.h"
#include <windowsx.h>
#include "Console.h"

GameWindow::GameWindow(const char *className, const char *title, int windowWidth, int windowHeight, bool fullscreen)
: Window(className, title, windowWidth, windowHeight, fullscreen)
{

}

void Window::show()
{
	ShowWindow(hwnd, SW_NORMAL);
	SetForegroundWindow(hwnd);                      // Slightly Higher Priority
	SetFocus(hwnd);                             // Sets Keyboard Focus To The Window

	CoreEngine engine(this);
	engine.run(hdc);
}

LRESULT GameWindow::windowEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)                  /* handle the messages */
	{
	case WM_ACTIVATE:
	{
		if (!HIWORD(wParam))
			active = true;
		else
			active = false;
		break;
	}
	case WM_SYSCOMMAND:                     // Intercept System Commands
	{
		switch (wParam)                     // Check System Calls
		{
		case SC_SCREENSAVE:
			return 0;
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}
	case WM_MOVE:
	{
		screenPos.setX(LOWORD(lParam));
		screenPos.setY(HIWORD(lParam));
		break;
	}
	case WM_KEYDOWN:
	{
		Keyboard::setKey((DWORD)wParam, true);
		break;
	}
	case WM_KEYUP:
	{
		Keyboard::setKey((DWORD)wParam, false);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		Mouse::setMouseButton(0, true);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		Mouse::setMouseButton(1, true);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		Mouse::setMouseButton(2, true);
		break;
	}
	case WM_XBUTTONDOWN:
	{
		if (GET_XBUTTON_WPARAM(wParam) == 1)
			Mouse::setMouseButton(3, true);
		else if (GET_XBUTTON_WPARAM(wParam) == 2)
			Mouse::setMouseButton(4, true);

		break;
	}
	case WM_LBUTTONUP:
	{
		Mouse::setMouseButton(0, false);
		break;
	}
	case WM_MBUTTONUP:
	{
		Mouse::setMouseButton(1, false);
		break;
	}
	case WM_RBUTTONUP:
	{
		Mouse::setMouseButton(2, false);
		break;
	}
	case WM_XBUTTONUP:
	{
		if (GET_XBUTTON_WPARAM(wParam) == 1)
			Mouse::setMouseButton(3, false);
		else if (GET_XBUTTON_WPARAM(wParam) == 2)
			Mouse::setMouseButton(4, false);

		break;
	}
	case WM_MOUSEMOVE:
	{
		Mouse::setMousePos(Vector2f((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam)));
		break;
	}
	/*case WM_INPUT:
	{
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
	lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
	int xPosRelative = raw->data.mouse.lLastX;
	int yPosRelative = raw->data.mouse.lLastY;
	Console::printf("%d %d\n", xPosRelative, yPosRelative);
	}
	break;
	}*/
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}