#include "Console.h"
#include <Windows.h>

void Console::initConsole()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
}

void Console::releaseConsole()
{
	FreeConsole();
}

void Console::printf(const char *format, ...)
{
	FILE *pFile = NULL;

	fopen_s(&pFile, "CON", "a");

	va_list args;
	va_start(args, format);
	vfprintf(pFile, format, args);
	va_end(args);

	fclose(pFile);
}