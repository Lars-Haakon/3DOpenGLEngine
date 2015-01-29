#include "Script.h"

Script::Script(char *filename)
{
	fopen_s(&m_file, filename, "r");
}

Script::~Script()
{
	fclose(m_file);
}

int Script::getIntData(char *data)
{
	char *tmp = data;
	int txtLength = 0;
	while (*tmp)
	{
		txtLength++;
		tmp++;
	}

	char *c = new char[txtLength + 4];
	for (int i = 0; i < txtLength; i++)
		c[i] = data[i];

	c[txtLength] = ' '; c[txtLength + 1] = '%'; c[txtLength + 2] = 'd'; c[txtLength + 3] = '\0';

	int ret = 0;
	while (fscanf_s(m_file, c, &ret) == 0)
	{
		while (fgetc(m_file) != '\n')
		{

		}
	}

	rewind(m_file);
	delete[] c;
	return ret;
}

float Script::getFloatData(char *data)
{
	char *tmp = data;
	int txtLength = 0;
	while (*tmp)
	{
		txtLength++;
		tmp++;
	}

	char *c = new char[txtLength + 4];
	for (int i = 0; i < txtLength; i++)
		c[i] = data[i];

	c[txtLength] = ' '; c[txtLength + 1] = '%'; c[txtLength + 2] = 'f'; c[txtLength + 3] = '\0';


	float ret = 0;
	while (fscanf_s(m_file, c, &ret) == 0)
	{
		while (fgetc(m_file) != '\n')
		{

		}
	}

	rewind(m_file);
	delete[] c;
	return ret;
}