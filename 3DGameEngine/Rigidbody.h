#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Mesh.h"
#include "Material.h"
#include "BoundingVolume.h"
#include "RK4Integrator.h"

class Rigidbody
{
public:
	Rigidbody(Mesh *mesh, Shader *shader, Material *material, BoundingVolume *boundingVolume, Matrix4f &inertiaTensor);
	~Rigidbody();

	void update();
	void render();

	State &getCurrent();
	State &getPrevious();
	BoundingVolume *getBoundingVolume();
	bool isAwake();
	static Rigidbody *loadRigidBody(const char *fileName);
private:
	Mesh *mesh;
	Shader *shader;
	Material *material;
	BoundingVolume *boundingVolume;

	Generator generator;
	State current;
	State previous;
	State interpolated;

	bool awake;
	double referenceTime; // Timer used for sleeping
};

#endif