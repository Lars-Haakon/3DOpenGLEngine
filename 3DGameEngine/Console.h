#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

class Console
{
public:
	static void initConsole();
	static void releaseConsole();

	static void printf(const char *format, ...);
};

#endif