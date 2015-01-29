#include "CollisionDetection.h"

#include "Console.h"
#include <math.h>

ContactData *CollisionDetection::generateCollisionContacts(Scene *scene)
{
	/*ContactData *data = new ContactData();

	for (int i = 0; i < objects->getLength(); i++)
	{
		Rigidbody *body = objects->get(i);

		if (body->isAwake())
		{
			LinkedList<BoundingBox> *boxes = &body->getBoundingVolume()->getBoxes();
			LinkedList<BoundingSphere> *spheres = &body->getBoundingVolume()->getSpheres();
			LinkedList<BoundingEllipsoid> *ellipsoids = &body->getBoundingVolume()->getEllipsoids();

			// Check terrain collision
			Face *sceneFaces = scene->getSceneFaces();
			int numSceneFaces = scene->getNumSceneFaces();

			for (int j = 0; j < numSceneFaces; j ++)
			{
				Face f = sceneFaces[j];

				for (int k = 0; k < boxes->getLength(); k++)
					checkFaceBox(data, f, body, boxes->get(k));
				for (int k = 0; k < spheres->getLength(); k++)
					checkFaceSphere(data, f, body, spheres->get(k));
				for (int k = 0; k < ellipsoids->getLength(); k++)
					checkFaceEllipsoid(data, f, body, ellipsoids->get(k));
			}
		}
	}*/

	return NULL;
}

void CollisionDetection::checkFaceBox(ContactData *data, Face &face, Rigidbody *body, BoundingBox *box)
{
	Vector3f vertices[8] = 
	{
		box->getCenter() + Vector3f(-box->getHalfsize().getX(), -box->getHalfsize().getY(), -box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(-box->getHalfsize().getX(), -box->getHalfsize().getY(), box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(-box->getHalfsize().getX(), box->getHalfsize().getY(), -box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(-box->getHalfsize().getX(), box->getHalfsize().getY(), box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(box->getHalfsize().getX(), -box->getHalfsize().getY(), -box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(box->getHalfsize().getX(), -box->getHalfsize().getY(), box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(box->getHalfsize().getX(), box->getHalfsize().getY(), -box->getHalfsize().getZ()),
		box->getCenter() + Vector3f(box->getHalfsize().getX(), box->getHalfsize().getY(), box->getHalfsize().getZ())
	};

	for (int i = 0; i < 8; i++)
	{
		Transform currTransform;
		currTransform.setPos(body->getCurrent().position);
		currTransform.setOrientation(body->getCurrent().orientation);

		Transform prevTransform;
		prevTransform.setPos(body->getPrevious().position);
		prevTransform.setOrientation(body->getPrevious().orientation);

		Vector3f currPos = currTransform.getTransformation() * vertices[i];
		Vector3f prevPos = prevTransform.getTransformation() * vertices[i];

		if ((face.p0 - currPos).dot(face.normal) >= 0 &&
			(face.p1 - face.p0).cross(currPos - face.p0).dot(face.normal) >= 0 &&
			(face.p2 - face.p1).cross(currPos - face.p1).dot(face.normal) >= 0 &&
			(face.p0 - face.p2).cross(currPos - face.p2).dot(face.normal) >= 0)
			//&& (v0 - prevPos).dot(FaceNormal) < 0)
		{
			/*if( (point1 - currPos).dot(planeNormal) > 0 && // check if current is on the wrong side of plane
			(point1 - prevPos).dot(planeNormal) <= 0 && // check if previous is on the correct side of plane
			(point2 - point1).cross(currPos - point1).dot(planeNormal) >= 0 &&
			(point3 - point2).cross(currPos - point2).dot(planeNormal) >= 0 &&
			(((point1 - point2) + point3) - point3).cross(currPos - point3).dot(planeNormal) >= 0 &&
			(point1 - ((point1 - point2) + point3)).cross(currPos - ((point1 - point2) + point3)).dot(planeNormal) >= 0)
			{*/

			float D = -face.p0.dot(face.normal);
			float penetration = fabs(currPos.dot(face.normal) + D); // PENETRATION IS POSITIVE IF THERE IS NO INTERSECTION

			data->contacts->add(new TerrainContact(currPos + face.normal * penetration, face.normal, penetration, 0.2f, body));
		}
	}
}

void CollisionDetection::checkFaceSphere(ContactData *data, Face &face, Rigidbody *body, BoundingSphere *sphere)
{

}

void CollisionDetection::checkFaceEllipsoid(ContactData *data, Face &face, Rigidbody *body, BoundingEllipsoid *ellipsoid)
{

}