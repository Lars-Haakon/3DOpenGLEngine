#ifndef CONTACT_H
#define CONTACT_H

#include "Rigidbody.h"
#include "Vector3f.h"

class Contact
{
public:
	Contact(Vector3f contactPoint, Vector3f contactNormal, float penetration, float restitution);

	virtual void calculateInternals() = 0;

	virtual void applyPositionChange(float max) = 0;
	virtual void updatePenetration() = 0;

	virtual void applyVelocityChange(float max) = 0;
	virtual void updateDesiredVelocity() = 0;

	float getPenetration();
	float getDesiredVelocity();
protected:
	// PRIMARY COLLISION INFORMASJON
	Vector3f contactPoint;
	Vector3f contactNormal;
	float penetration;
	float restitution;

	// SECONDARY
	Vector3f relativeContactPos;
	Vector3f contactVelocity;
	float desiredVelocity;
	Vector3f linearChange;
	Vector3f angularChange;
	Vector3f linearVelocityChange;
	Vector3f angularVelocityChange;
};

class TerrainContact : public Contact
{
public:
	TerrainContact(Vector3f contactPoint, Vector3f contactNormal, float penetration, float restitution, Rigidbody *body);

	virtual void calculateInternals();

	virtual void applyPositionChange(float max);
	virtual void updatePenetration();

	virtual void applyVelocityChange(float max);
	virtual void updateDesiredVelocity();

	virtual Rigidbody *getBody1();
	virtual Rigidbody *getBody2();
private:
	Rigidbody *body; // First body of the contact
};

//class BodyContact : public Contact
//{
//public:
//	BodyContact(Vector3f contactPoint, Vector3f contactNormal, float penetration, Rigidbody *body1, Rigidbody *body2);
//
//	virtual void calculateInternals(float restitution);
//private:
//	// one of these boddies can be NULL. if it is then it is a terrain-collision
//	Rigidbody *body1; // First body of the contact
//	Rigidbody *body2; // Second body of the contact
//};

#endif