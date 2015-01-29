#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Vector3f.h"
#include "Transform.h"
#include "ForwardAmbient.h"

/*
box - box
ellipsoid - ellipsoid
sphere - sphere
box - ellipsoid
ellipsoid - sphere
sphere - box
*/

class BoundingBox
{
public:
	BoundingBox(Vector3f center, Vector3f halfsize);
	~BoundingBox();

	Vector3f getCenter();
	Vector3f getHalfsize();

	void draw(Transform &transform);
private:
	ForwardAmbient *shader;

	Vector3f center;
	Vector3f halfsize;

	unsigned int m_vbo;
	int m_size;
};

class BoundingSphere
{
public:
	BoundingSphere(Vector3f center, float radius);
	~BoundingSphere();

	void draw(Transform &transform);
private:
	ForwardAmbient *shader;

	Vector3f center;
	float radius;

	unsigned int m_vbo;
	int m_size_xy;
	int m_size_yz;
	int m_size_zx;
};

class BoundingEllipsoid
{
public:
	BoundingEllipsoid(Vector3f center, Vector3f halfsize);
	~BoundingEllipsoid();

	void draw(Transform &transform);
private:
	ForwardAmbient *shader;

	Vector3f center;
	Vector3f halfsize;

	unsigned int m_vbo;
	int m_size_xy;
	int m_size_yz;
	int m_size_zx;
};

class BoundingVolume
{
public:
	BoundingVolume();

	void addBox(BoundingBox *box);
	void addSphere(BoundingSphere *sphere);
	void addEllipsoid(BoundingEllipsoid *ellipsoid);

	void draw(Transform &transform);

	LinkedList<BoundingBox> &getBoxes();
	LinkedList<BoundingSphere> &getSpheres();
	LinkedList<BoundingEllipsoid> &getEllipsoids();
private:
	LinkedList<BoundingBox> boxes; // List of boxes in boundingvolume
	LinkedList<BoundingSphere> spheres; // List of spheres in boundingvolume
	LinkedList<BoundingEllipsoid> ellipsoids; // List of ellipsoids in boundingvolume
};

#endif