#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>

class Script
{
public:
	Script(char *filename);
	~Script();

	int getIntData(char *data);
	float getFloatData(char *data);
private:
	FILE *m_file;
};

#endif