#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Contact.h"
#include "Geometry.h"
#include "Scene.h"

struct ContactData
{
	LinkedList<Contact> *contacts;

	ContactData()
	{
		contacts = new LinkedList<Contact>();
	}

	~ContactData()
	{
		delete contacts;
	}
};

class CollisionDetection
{
	/*
	box - box
	sphere - sphere
	ellipsoid - ellipsoid
	box - sphere
	ellipsoid - box
	sphere - ellipsoid
	*/
public:
	static ContactData *generateCollisionContacts(Scene *scene);
private:
	static void checkFaceBox(ContactData *data, Face &face, Rigidbody *body, BoundingBox *box);
	static void checkFaceSphere(ContactData *data, Face &face, Rigidbody *body, BoundingSphere *sphere);
	static void checkFaceEllipsoid(ContactData *data, Face &face, Rigidbody *body, BoundingEllipsoid *ellipsoid);
	/*static void checkBoxBox(LinkedList<Contact> &contacts, Rigidbody *body1, Rigidbody *body2, BoundingBox &box1, BoundingBox &box2);
	static void checkSphereSphere(LinkedList<Contact> &contacts, Rigidbody *body1, Rigidbody *body2, BoundingSphere &sphere1, BoundingSphere &sphere2);
	static void checkEllipsoidEllipsoid(LinkedList<Contact> &contacts, Rigidbody *body1, Rigidbody *body2, BoundingEllipsoid &ellipsoid1, BoundingEllipsoid &ellipsoid2);
	static void checkBoxSphere();
	static void checkEllipsoidBox();
	static void checkSphereEllipsoid();*/
};

#endif