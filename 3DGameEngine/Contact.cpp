#include "Contact.h"

Contact::Contact(Vector3f contactPoint, Vector3f contactNormal, float penetration, float restitution)
{
	this->contactPoint = contactPoint;
	this->contactNormal = contactNormal;
	this->penetration = penetration;
	this->restitution = restitution;
}

float Contact::getPenetration()
{
	return penetration;
}

float Contact::getDesiredVelocity()
{
	return desiredVelocity;
}

TerrainContact::TerrainContact(Vector3f contactPoint, Vector3f contactNormal, float penetration, float restitution, Rigidbody *body)
: Contact(contactPoint, contactNormal, penetration, restitution)
{
	this->body = body;
}

void TerrainContact::calculateInternals()
{
	Vector3f origin = body->getCurrent().position;
	Vector3f velocity = body->getCurrent().velocity;
	Vector3f angularVelocity = body->getCurrent().angularVelocity;

	relativeContactPos = contactPoint - origin;
	contactVelocity = angularVelocity.cross(relativeContactPos) + velocity; // the velocity of the collision point
	desiredVelocity = (-contactVelocity * (1 + restitution)).dot(contactNormal);
}

void TerrainContact::applyPositionChange(float max)
{
	Matrix4f r;
	r.initRotateTransform(body->getCurrent().orientation.getX(), body->getCurrent().orientation.getY(), body->getCurrent().orientation.getZ(), body->getCurrent().orientation.getW());
	Matrix4f inverseInertiaTensorWorld = r * body->getCurrent().inertiaTensor.inverted() * r.transposed();

	float linearInertia = 1 / body->getCurrent().mass;
	float angularInertia = ((inverseInertiaTensorWorld * relativeContactPos.cross(contactNormal)).cross(relativeContactPos)).dot(contactNormal);
	float totalInertia = linearInertia + angularInertia;

	float linearMove = max * (linearInertia / totalInertia);
	float angularMove = max * (angularInertia / totalInertia);

	// To avoid angular projections that are too great (when mass is large
	// but inertia tensor is small) limit the angular move.
	Vector3f projection = contactPoint;
	projection += -contactPoint.cross(contactNormal) * contactNormal;

	// Use the small angle approximation for the sine of the angle (i.e.
	// the magnitude would be sine(angularLimit) * projection.magnitude
	// but we approximate sine(angularLimit) to angularLimit) 0.2f.
	float maxMagnitude = 0.2f * projection.length();

	if (angularMove < -maxMagnitude)
	{
		float totalMove = angularMove + linearMove;
		angularMove = -maxMagnitude;
		linearMove = totalMove - angularMove;
	}
	else if (angularMove > maxMagnitude)
	{
		float totalMove = angularMove + linearMove;
		angularMove = maxMagnitude;
		linearMove = totalMove - angularMove;
	}

	linearChange = contactNormal * linearMove;
	angularChange = inverseInertiaTensorWorld * relativeContactPos.cross(contactNormal) * (angularMove / angularInertia);

	body->getCurrent().position += linearChange;
	body->getCurrent().orientation += (Quaternion(angularChange.getX(), angularChange.getY(), angularChange.getZ(), 0) * body->getCurrent().orientation) / 2.0f;
	body->getCurrent().orientation.normalize();
}

void TerrainContact::updatePenetration()
{
	Vector3f deltaPosition = linearChange + angularChange.cross(relativeContactPos);

	// The sign of the change is positive if we're
	// dealing with the second body in a contact
	// and negative otherwise (because we're
	// subtracting the resolution)..
	penetration += -deltaPosition.dot(contactNormal);

	//data.contacts[i].setRelativeContactPos(data.contacts[i].getRelativeContactPos() + deltaPosition);
}

void TerrainContact::applyVelocityChange(float max)
{
	Matrix4f r;
	r.initRotateTransform(body->getCurrent().orientation.getX(), body->getCurrent().orientation.getY(), body->getCurrent().orientation.getZ(), body->getCurrent().orientation.getW());
	Matrix4f inverseInertiaTensorWorld = r * body->getCurrent().inertiaTensor.inverted() * r.transposed();

	float J = max / (1 / body->getCurrent().mass + (inverseInertiaTensorWorld * relativeContactPos.cross(contactNormal)).cross(relativeContactPos).dot(contactNormal));

	linearVelocityChange = (contactNormal * J) / body->getCurrent().mass;
	angularVelocityChange = inverseInertiaTensorWorld * relativeContactPos.cross(contactNormal * J);

	body->getCurrent().velocity += linearVelocityChange;
	body->getCurrent().angularVelocity += angularVelocityChange;
}

void TerrainContact::updateDesiredVelocity()
{
	Vector3f deltaVel = linearVelocityChange + angularVelocityChange.cross(relativeContactPos);

	// The sign of the change is negative if we're dealing
	// with the second body in a contact.
	contactVelocity += deltaVel;
	desiredVelocity = (-contactVelocity * (1 + restitution)).dot(contactNormal);
}

Rigidbody *TerrainContact::getBody1()
{
	return body;
}

Rigidbody *TerrainContact::getBody2()
{
	return NULL;
}