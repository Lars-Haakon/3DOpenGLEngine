#ifndef TIME_H
#define TIME_H

class Time
{
public:
	static double startCounter();
	static double getSystemTime();

	static void update();

	static double getTime();
	static double getDeltaTime();
	static void setDeltaTime(double deltaTime);
	static double getAlpha();
	static void setAlpha(double alpha);
private:
	static __int64 PCFreq;
	static __int64 CounterStart;

	static double time;
	static double deltaTime;
	static double alpha;
};

#endif