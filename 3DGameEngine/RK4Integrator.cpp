#include "RK4Integrator.h"

void RK4Integrator::interpolate(State &previous, State &current, State &interpolated, float alpha)
{
	interpolated = current;

	interpolated.acceleration = previous.acceleration.lerp(current.acceleration, alpha);
	interpolated.velocity = previous.velocity.lerp(current.velocity, alpha);
	interpolated.position = previous.position.lerp(current.position, alpha);
	interpolated.orientation = previous.orientation.slerp(current.orientation, alpha);
	interpolated.angularAcceleration = previous.angularAcceleration.lerp(current.angularAcceleration, alpha);
	interpolated.angularVelocity = previous.angularVelocity.lerp(current.angularVelocity, alpha);
}

void RK4Integrator::integrate(State &current, float dt)
{
	Derivative a = evaluate(current);
	Derivative b = evaluate(current, a, dt * 0.5f);
	Derivative c = evaluate(current, b, dt * 0.5f);
	Derivative d = evaluate(current, c, dt);

	current.velocity += (a.dAcceleration + (b.dAcceleration + c.dAcceleration) * 2.0f + d.dAcceleration) * (dt / 6);
	current.position += (a.dVelocity + (b.dVelocity + c.dVelocity) * 2.0f + d.dVelocity) * (dt / 6);
	current.orientation += (a.spin + (b.spin + c.spin) * 2.0f + d.spin) * (dt / 6.0f);
	current.orientation.normalize();
	current.angularVelocity += (a.dAngularAcceleration + (b.dAngularAcceleration + c.dAngularAcceleration) * 2.0f + d.dAngularAcceleration) * (dt / 6);
}

Derivative RK4Integrator::evaluate(State &current)
{
	Derivative output;
	output.dAcceleration = current.acceleration;
	output.dVelocity = current.velocity;
	output.spin = (Quaternion(current.angularVelocity.getX(), current.angularVelocity.getY(), current.angularVelocity.getZ(), 0.0f) * current.orientation) / 2.0f;
	output.dAngularAcceleration = current.angularAcceleration;
	return output;
}

Derivative RK4Integrator::evaluate(State &current, Derivative &d, float dt)
{
	State state = current;
	state.velocity += d.dAcceleration * dt;
	state.position += d.dVelocity * dt;
	state.orientation += d.spin * dt;
	state.orientation.normalize();
	state.angularVelocity += d.dAngularAcceleration * dt;

	Derivative output;
	output.dAcceleration = state.acceleration;
	output.dVelocity = state.velocity;
	output.spin = (Quaternion(state.angularVelocity.getX(), state.angularVelocity.getY(), state.angularVelocity.getZ(), 0.0f) * state.orientation) / 2.0f;
	output.dAngularAcceleration = state.angularAcceleration;
	return output;
}