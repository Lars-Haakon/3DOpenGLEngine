#include "Window.h"
#include "Console.h"

HashTable<Window> Window::windows(5);
HINSTANCE Window::hInstance;

Window::Window(const char *className, const char *title, int windowWidth, int windowHeight, bool _fullscreen)
{
	this->className = className;
	this->title = title;
	this->fullscreen = _fullscreen;
	this->width = windowWidth;
	this->height = windowHeight;

	/* The Window structure */
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpfnWndProc = WindowProc;      /* This function is called by windows */
	wc.lpszClassName = className;
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC;

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wc))
		MessageBox(HWND_DESKTOP, "Failed to register class", "Error", MB_OK);

	DWORD dwStyle = WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU;	// | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. returning to window mode", "FULLSCREEN ERROR", MB_OK);
			fullscreen = false;		// Windowed Mode Selected.  Fullscreen = FALSE
		}
		else
			dwStyle = WS_POPUP;
	}

	RECT winRect;
	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, dwStyle, false);

	windows.put((char*)className, this);
	hwnd = CreateWindow(
		className,         /* Classname */
		title,       /* Title Text */
		dwStyle, /* default window */
		(GetSystemMetrics(SM_CXSCREEN) - winRect.right) / 2,       /* center the X-axis */
		(GetSystemMetrics(SM_CYSCREEN) - winRect.bottom) / 2,       /* center the Y-axis */
		winRect.right - winRect.left,                 /* The programs width */
		winRect.bottom - winRect.top,                 /* and height in pixels */
		NULL,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
		);

	if (!(hdc = GetDC(hwnd)))							// Did We Get A Device Context?
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	if (!setupPixelFormat())	// Are We Able To Set The Pixel Format?
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	if (!(hrc = wglCreateContext(hdc)))				// Are We Able To Get A Rendering Context?
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	if (!wglMakeCurrent(hdc, hrc))					// Try To Activate The Rendering Context
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	/*// Register listener for RAW INPUT
	USHORT HID_USAGE_PAGE_GENERIC = 0x01;
	USHORT HID_USAGE_GENERIC_MOUSE = 0x02;

	RAWINPUTDEVICE rid;
	rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd;
	RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));*/
}

Window::~Window()
{
	if (fullscreen)										// Are We In Fullscreen Mode?
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop

	if (hrc)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hrc))						// Are We Able To Delete The RC?
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hrc = NULL;										// Set RC To NULL
	}

	if (hdc && !ReleaseDC(hwnd,hdc))                    // Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hdc=NULL;                           // Set DC To NULL
	}

	if (hwnd && !DestroyWindow(hwnd))                   // Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hwnd=NULL;                          // Set hWnd To NULL
	}

	hdc = NULL;
	hwnd = NULL;

	if (!UnregisterClass(className, hInstance))			// Are We Able To Unregister Class
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
}

bool Window::setupPixelFormat()
{
	/*      Pixel format index
	*/

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	/*      Choose best matching format*/
	int nPixelFormat = 0;
	if (!(nPixelFormat = ChoosePixelFormat(hdc, &pfd)))
		return false;

	/*      Set the pixel format to the device context*/
	if (!SetPixelFormat(hdc, nPixelFormat, &pfd))
		return false;

	return true;
}

void Window::setInstance(HINSTANCE instance)
{
	hInstance = instance;
}

int Window::getScreenWidth()
{
	return width;
}

int Window::getScreenHeight()
{
	return height;
}

Vector2f Window::getScreenPos()
{
	return screenPos;
}

bool Window::isActive()
{
	return active;
}

const char *Window::getClassName()
{
	return className;
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char className[50];
	GetClassName(hwnd, className, 50);
	Window *window = windows.get(className);
	if (window != NULL)
	{
		if (uMsg == WM_CREATE)
		{

		}
		else if (uMsg == WM_DESTROY)
		{
			PostQuitMessage(0);
			windows.remove(className);
		}
		else if (uMsg == WM_GETMINMAXINFO || uMsg == WM_NCCREATE || uMsg == WM_NCCALCSIZE){}
		else
			return window->windowEvent(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}