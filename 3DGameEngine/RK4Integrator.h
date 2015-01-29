#ifndef RK4INTEGRATOR_H
#define RK4INTEGRATOR_H

#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4f.h"

struct Generator
{
	Vector3f forceAccumulator;
	Vector3f torqueAccumulator;

	void addForce(Vector3f &force)
	{
		forceAccumulator += force;
	}

	void addTorque(Vector3f &torque)
	{
		torqueAccumulator += torque;
	}

	void clear()
	{
		forceAccumulator = Vector3f(0, 0, 0);
		torqueAccumulator = Vector3f(0, 0, 0);
	}
};

struct State
{
	// Primary physics state
	Vector3f acceleration;
	Vector3f velocity;
	Vector3f position;
	Quaternion orientation;
	Vector3f angularAcceleration;
	Vector3f angularVelocity;

	// Constant physics state
	float mass;
	Matrix4f inertiaTensor;
};

struct Derivative
{
	Vector3f dAcceleration; // derivate of velocity
	Vector3f dVelocity; // derivate of position
	Quaternion spin; // derivate of orientation
	Vector3f dAngularAcceleration; // derivate of angular velocity
};

class RK4Integrator
{
public:
	static void interpolate(State &previous, State &current, State &interpolated, float alpha);
	static void integrate(State &current, float dt);

private:
	static Derivative evaluate(State &current);
	static Derivative evaluate(State &current, Derivative &d, float dt);
};

#endif