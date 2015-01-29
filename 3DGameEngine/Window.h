#ifndef WINDOW_H
#define WINDOW_H

#include "HashTable.h"
#include "Vector2f.h"
#include <Windows.h>

class Window
{
public:
	Window(const char *className, const char *title, int width, int height, bool fullscreen);
	~Window();

	static void setInstance(HINSTANCE instance);

	int getScreenWidth();
	int getScreenHeight();
	Vector2f getScreenPos();
	bool isActive();

	const char *getClassName();
	
	void show();
	virtual LRESULT windowEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
protected:
	HWND hwnd;
	HDC hdc;
	Vector2f screenPos;
	bool active;
	
	void setupWindow();
	
private:
	static HINSTANCE hInstance; // APPLICATION INSTANCE
	static HashTable<Window> windows;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool setupPixelFormat();

	int width, height;
	const char *className;
	const char *title;
	bool fullscreen;
	
	HGLRC hrc;
};

#endif