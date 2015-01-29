#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "RK4Integrator.h"

class GameObject
{
public:

private:
	Generator generator;
	State current;
	State previous;
	State interpolated;
};

#endif