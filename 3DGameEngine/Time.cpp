#include "Time.h"
#include <Windows.h>

__int64 Time::PCFreq = 0;
__int64 Time::CounterStart = 0;

double Time::time = 0.0;
double Time::deltaTime = 0.0;
double Time::alpha = 0.0;

double Time::startCounter()
{
	if (!QueryPerformanceCounter((LARGE_INTEGER*)&CounterStart))
		MessageBox(NULL, "QueryPerformanceCounter failed!!!", "ERROR", MB_OK);

	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&PCFreq))
		MessageBox(NULL, "QueryPerformanceFrequency failed!!!", "ERROR", MB_OK);

	return 0.0;
}

double Time::getSystemTime()
{
	__int64 counter = 0;
	if (!QueryPerformanceCounter((LARGE_INTEGER*)&counter))
		MessageBox(NULL, "QueryPerformanceCounter failed!!!", "ERROR", MB_OK);

	return ((counter - CounterStart) / (double)PCFreq);
}

void Time::update()
{
	time += deltaTime;
}

double Time::getTime()
{
	return time;
}

double Time::getDeltaTime()
{
	return deltaTime;
}

void Time::setDeltaTime(double deltaTime)
{
	Time::deltaTime = deltaTime;
}

double Time::getAlpha()
{
	return alpha;
}

void Time::setAlpha(double alpha)
{
	Time::alpha = alpha;
}