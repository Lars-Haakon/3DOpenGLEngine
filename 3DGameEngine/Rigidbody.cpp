#include "Rigidbody.h"
#include "CollisionDetection.h"
#include "Console.h"
#include "Time.h"

Rigidbody::Rigidbody(Mesh *mesh, Shader *shader, Material *material, BoundingVolume *boundingVolume, Matrix4f &inertiaTensor)
{
	this->mesh = mesh;
	this->shader = shader;
	this->material = material;
	this->boundingVolume = boundingVolume;

	current.acceleration = Vector3f(0.0f, 0.0f, 0.0f);
	current.velocity = Vector3f(0.0f, 0.0f, 0.0f);
	current.position = Vector3f(0.0f, 120, 0.0f);
	current.orientation = Quaternion(0, -0.7071067811865476f, 0, 0.7071067811865476f).normalized();
	current.angularAcceleration = Vector3f(0.0f, 0.0f, 0.0f);
	current.angularVelocity = Vector3f(0.0f, 0.0f, 0.0f);
	current.mass = 300.0f;
	current.inertiaTensor = inertiaTensor * current.mass; // the inertia tensor is passed by reference. this is OK since we multiply it to recieve a new matrix object
	previous = current;

	awake = true;
	referenceTime = Time::getTime();
}

Rigidbody::~Rigidbody()
{
	delete mesh;
	delete shader;
	delete material;
	delete boundingVolume;
}

void Rigidbody::update()
{
	if (awake)
	{
		float vLength = current.velocity.length();
		float avLength = current.angularVelocity.length();
		float motion = vLength * vLength + avLength * avLength;
		if (motion < 1.0f)
		{
			if (Time::getTime() - referenceTime > 0.2f)
				awake = false;
		}
		else
			referenceTime = Time::getTime();

		previous = current;
		//generator.addForce(Vector3f(0.0f, -10.0f, 0.0f) * current.mass); // gravity 
		
		current.acceleration = generator.forceAccumulator / current.mass; // update acceleration based on forces
		current.angularAcceleration = current.inertiaTensor * generator.torqueAccumulator;
		generator.clear();

		RK4Integrator::integrate(current, (float) Time::getDeltaTime());
	}
}

BoundingVolume *Rigidbody::getBoundingVolume()
{
	return boundingVolume;
}

State &Rigidbody::getCurrent()
{
	return current;
}

State &Rigidbody::getPrevious()
{
	return previous;
}

bool Rigidbody::isAwake()
{
	return awake;
}

void Rigidbody::render()
{
	if (awake)
		RK4Integrator::interpolate(previous, current, interpolated, (float) Time::getAlpha());

	Transform transform;
	transform.setPos(interpolated.position);
	transform.setOrientation(interpolated.orientation);

	shader->bind();
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), material);
	mesh->draw();
	boundingVolume->draw(transform);
}

Rigidbody *Rigidbody::loadRigidBody(const char *fileName) 
{
	FILE *pFile = NULL;
	fopen_s(&pFile, fileName, "r");

	if (pFile == NULL)
	{
		Console::printf("%s\n", "Could not open file");
		exit(1);
	}

	Matrix4f inertiaTensor;
	BoundingVolume *boundingVolume = new BoundingVolume();

	LinkedList<Vector3f> vertices;
	LinkedList<Vector3f> normals;
	LinkedList<Vector2f> texCoords;
	LinkedList<int> indices;

	while (!feof(pFile))
	{
		char line[80];
		fgets(line, 80, pFile);

		if (line[0] == '#') //if it's a comment
			continue;       //we don't have to do anything with it
		else if (line[0] == 'b' && line[1] == 'v')
		{
			char type[10];
			fgets(type, 10, pFile);
			int t;
			sscanf_s(type, "Type %d", &t);
			if (t == 0)
			{
				char center[80];
				fgets(center, 80, pFile);
				float cx, cy, cz;
				sscanf_s(center, "Center %f %f %f", &cx, &cy, &cz);

				char halfsize[80];
				fgets(halfsize, 80, pFile);
				float hx, hy, hz;
				sscanf_s(halfsize, "Halfsize %f %f %f", &hx, &hy, &hz);

				boundingVolume->addBox(new BoundingBox(Vector3f(cx, cy, cz), Vector3f(hx, hy, hz)));
			}
			else if (t == 1)
			{
				char center[80];
				fgets(center, 80, pFile);
				float cx, cy, cz;
				sscanf_s(center, "Center %f %f %f", &cx, &cy, &cz);

				char radius[80];
				fgets(radius, 80, pFile);
				float r;
				sscanf_s(radius, "Radius %f", &r);

				boundingVolume->addSphere(new BoundingSphere(Vector3f(cx, cy, cz), r));
			}
			else if (t == 2)
			{
				char center[80];
				fgets(center, 80, pFile);
				float cx, cy, cz;
				sscanf_s(center, "Center %f %f %f", &cx, &cy, &cz);

				char halfsize[80];
				fgets(halfsize, 80, pFile);
				float hx, hy, hz;
				sscanf_s(halfsize, "Halfsize %f %f %f", &hx, &hy, &hz);

				boundingVolume->addEllipsoid(new BoundingEllipsoid(Vector3f(cx, cy, cz), Vector3f(hx, hy, hz)));
			}
		}
		else if (line[0] == 'i' && line[1] == 't')
		{
			char row[80];
			fgets(row, 80, pFile);
			sscanf_s(row, "%f %f %f %f", &inertiaTensor[0][0], &inertiaTensor[0][1], &inertiaTensor[0][2], &inertiaTensor[0][3]);
			fgets(row, 80, pFile);
			sscanf_s(row, "%f %f %f %f", &inertiaTensor[1][0], &inertiaTensor[1][1], &inertiaTensor[1][2], &inertiaTensor[1][3]);
			fgets(row, 80, pFile);
			sscanf_s(row, "%f %f %f %f", &inertiaTensor[2][0], &inertiaTensor[2][1], &inertiaTensor[2][2], &inertiaTensor[2][3]);
			fgets(row, 80, pFile);
			sscanf_s(row, "%f %f %f %f", &inertiaTensor[3][0], &inertiaTensor[3][1], &inertiaTensor[3][2], &inertiaTensor[3][3]);
		}
		else if (line[0] == 'v' && line[1] == ' ')     //if a vertex
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(line, "v %f %f %f", &tmpx, &tmpy, &tmpz);       //read the 3 floats, which makes up the vertex
			vertices.add(new Vector3f(tmpx, tmpy, tmpz));				//and put it in the vertex vector
		}
		else if (line[0] == 'v' && line[1] == 'n')    //if it's a normal vector
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(line, "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.add(new Vector3f(tmpx, tmpy, tmpz));      //basically do the same
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			float tmpx, tmpy;
			sscanf_s(line, "vt %f %f", &tmpx, &tmpy);
			texCoords.add(new Vector2f(tmpx, tmpy));      //basically do the same
		}
		else if (line[0] == 'f')   //if it's a face
		{
			int *a = new int, *b = new int, *c = new int;
			sscanf_s(line, "f %d %d %d\n", a, b, c);

			indices.add(a);
			indices.add(b);
			indices.add(c);
		}
	}
	fclose(pFile);

	Mesh *mesh = new Mesh();
	Vertex *vert = new Vertex[vertices.getLength()];
	int *ind = indices.toArray();

	if (normals.getLength() != 0)
	{
		for (int i = 0; i < vertices.getLength(); i++)
		{
			vert[i].pos = *vertices.get(i);
			//vert[i].normal = *normals.get(i);
			vert[i].texCoord = *texCoords.get(i);
		}

		mesh->addVertices(vert, ind, vertices.getLength(), indices.getLength(), false);
	}
	else
	{
		for (int i = 0; i < vertices.getLength(); i++)
		{
			vert[i].pos = *vertices.get(i);
			//vert[i].normal = Vector3f(0, 0, 0);
			vert[i].texCoord = *texCoords.get(i);
		}

		mesh->addVertices(vert, ind, vertices.getLength(), indices.getLength(), true);
	}
	delete[] vert;
	delete[] ind;

	return new Rigidbody(mesh, new ForwardAmbient(), new Material(Texture::loadBMP("textures/mp5.bmp"), NULL, 8.0f, 32.0f), boundingVolume, inertiaTensor);
}