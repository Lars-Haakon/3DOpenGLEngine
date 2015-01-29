#include "BoundingVolume.h"
#include "glExtFunc.h"
#include <math.h>
#include "Console.h"

BoundingVolume::BoundingVolume()
{

}

void BoundingVolume::addBox(BoundingBox *box)
{
	boxes.add(box);
}

void BoundingVolume::addSphere(BoundingSphere *sphere)
{
	spheres.add(sphere);
}

void BoundingVolume::addEllipsoid(BoundingEllipsoid *ellipsoid)
{
	ellipsoids.add(ellipsoid);
}

void BoundingVolume::draw(Transform &transform)
{
	for (int i = 0; i < boxes.getLength(); i++)
		boxes.get(i)->draw(transform);
	for (int i = 0; i < spheres.getLength(); i++)
		spheres.get(i)->draw(transform);
	for (int i = 0; i < ellipsoids.getLength(); i++)
		ellipsoids.get(i)->draw(transform);
}

LinkedList<BoundingBox> &BoundingVolume::getBoxes()
{
	return boxes;
}

LinkedList<BoundingSphere> &BoundingVolume::getSpheres()
{
	return spheres;
}

LinkedList<BoundingEllipsoid> &BoundingVolume::getEllipsoids()
{
	return ellipsoids;
}

BoundingBox::BoundingBox(Vector3f _center, Vector3f _halfsize)
{
	this->center = _center;
	this->halfsize = _halfsize;

	glGenBuffersARB(1, &m_vbo);

	LinkedList<Vector3f> vertexList;
	Vector3f v = center + Vector3f(-halfsize.getX(), halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));

	v = center + Vector3f(-halfsize.getX(), -halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), -halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), -halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), -halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), -halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));

	v = center + Vector3f(-halfsize.getX(), halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), -halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(-halfsize.getX(), -halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), -halfsize.getY(), halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	v = center + Vector3f(halfsize.getX(), -halfsize.getY(), -halfsize.getZ());
	vertexList.add(new Vector3f(v.getX(), v.getY(), v.getZ()));

	m_size = vertexList.getLength();
	Vector3f *vertexData = vertexList.toArray();

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_size * sizeof(Vector3f), vertexData, GL_STATIC_DRAW_ARB);
	delete[] vertexData;
}

BoundingBox::~BoundingBox()
{
	delete shader;

	glDeleteBuffersARB(1, &m_vbo);
}

Vector3f BoundingBox::getCenter()
{
	return center;
}

Vector3f BoundingBox::getHalfsize()
{
	return halfsize;
}

void BoundingBox::draw(Transform &transform)
{
	shader->bind();
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);

	glEnableVertexAttribArrayARB(0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
	glDrawArrays(GL_LINES, 0, m_size);

	glDisableVertexAttribArrayARB(0);
}

BoundingSphere::BoundingSphere(Vector3f _center, float _radius)
{
	shader = new ForwardAmbient();

	this->center = _center;
	this->radius = _radius;

	glGenBuffersARB(1, &m_vbo);

	LinkedList<Vector3f> vertices;

	float step = 0.1f;

	for (float x = -radius; x < radius; x += step)
	{
		float y = sqrtf(fabs(radius * radius - x * x));

		Vector3f v = center + Vector3f(x, y, 0);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float x = -radius; x < radius; x += step)
	{
		float y = sqrtf(fabs(radius * radius - x * x));

		Vector3f v = center + Vector3f(-x, -y, 0);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_xy = vertices.getLength();

	for (float y = -radius; y < radius; y += step)
	{
		float z = sqrtf(fabs(radius * radius - y * y));

		Vector3f v = center + Vector3f(0, y, z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float y = -radius; y < radius; y += step)
	{
		float z = sqrtf(fabs(radius * radius - y * y));

		Vector3f v = center + Vector3f(0, -y, -z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_yz = vertices.getLength() - m_size_xy;

	for (float z = -radius; z < radius; z += step)
	{
		float x = sqrtf(fabs(radius * radius - z * z));

		Vector3f v = center + Vector3f(x, 0, z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float z = -radius; z < radius; z += step)
	{
		float x = sqrtf(fabs(radius * radius - z * z));

		Vector3f v = center + Vector3f(-x, 0, -z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_zx = vertices.getLength() - (m_size_xy + m_size_yz);

	Vector3f *vertexData = vertices.toArray();

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (m_size_xy + m_size_yz + m_size_zx) * sizeof(Vector3f), vertexData, GL_STATIC_DRAW_ARB);
	delete[] vertexData;
}

BoundingSphere::~BoundingSphere()
{
	delete shader;

	glDeleteBuffersARB(1, &m_vbo);
}

void BoundingSphere::draw(Transform &transform)
{
	glEnableVertexAttribArrayARB(0);

	shader->bind();
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);

	//shader->setColor(Vector3f(1, 0, 0));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, 0, m_size_xy);

	//shader->setColor(Vector3f(0, 1, 0));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, m_size_xy, m_size_yz);

	//shader->setColor(Vector3f(0, 0, 1));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, m_size_xy + m_size_yz, m_size_zx);

	glDisableVertexAttribArrayARB(0);
}

BoundingEllipsoid::BoundingEllipsoid(Vector3f _center, Vector3f _halfsize)
{
	shader = new ForwardAmbient();

	this->center = _center;
	this->halfsize = _halfsize;

	glGenBuffersARB(1, &m_vbo);

	LinkedList<Vector3f> vertices;

	float step = 0.1f;

	for (float x = -halfsize.getX(); x < halfsize.getX(); x += step)
	{
		float y = sqrtf(fabs(halfsize.getY() * halfsize.getY() - (halfsize.getY() * halfsize.getY() * x * x / (halfsize.getX() * halfsize.getX()))));

		Vector3f v = center + Vector3f(x, y, 0);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float x = -halfsize.getX(); x < halfsize.getX(); x += step)
	{
		float y = sqrtf(fabs(halfsize.getY() * halfsize.getY() - (halfsize.getY() * halfsize.getY() * x * x / (halfsize.getX() * halfsize.getX()))));

		Vector3f v = center + Vector3f(-x, -y, 0);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_xy = vertices.getLength();

	for (float y = -halfsize.getY(); y < halfsize.getY(); y += step)
	{
		float z = sqrtf(fabs(halfsize.getZ() * halfsize.getZ() - (halfsize.getZ() * halfsize.getZ() * y * y / (halfsize.getY() * halfsize.getY()))));

		Vector3f v = center + Vector3f(0, y, z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float y = -halfsize.getY(); y < halfsize.getY(); y += step)
	{
		float z = sqrtf(fabs(halfsize.getZ() * halfsize.getZ() - (halfsize.getZ() * halfsize.getZ() * y * y / (halfsize.getY() * halfsize.getY()))));

		Vector3f v = center + Vector3f(0, -y, -z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_yz = vertices.getLength() - (m_size_xy);

	for (float z = -halfsize.getZ(); z < halfsize.getZ(); z += step)
	{
		float x = sqrtf(fabs(halfsize.getX() * halfsize.getX() - (halfsize.getX() * halfsize.getX() * z * z / (halfsize.getZ() * halfsize.getZ()))));

		Vector3f v = center + Vector3f(x, 0, z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}
	for (float z = -halfsize.getZ(); z < halfsize.getZ(); z += step)
	{
		float x = sqrtf(fabs(halfsize.getX() * halfsize.getX() - (halfsize.getX() * halfsize.getX() * z * z / (halfsize.getZ() * halfsize.getZ()))));

		Vector3f v = center + Vector3f(-x, 0, -z);
		vertices.add(new Vector3f(v.getX(), v.getY(), v.getZ()));
	}

	m_size_zx = vertices.getLength() - (m_size_xy + m_size_yz);
	Vector3f *vertexData = vertices.toArray();

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (m_size_xy + m_size_yz + m_size_zx) * sizeof(Vector3f), vertexData, GL_STATIC_DRAW_ARB);

	delete[] vertexData;
}

BoundingEllipsoid::~BoundingEllipsoid()
{
	delete shader;

	glDeleteBuffersARB(1, &m_vbo);
}

void BoundingEllipsoid::draw(Transform &transform)
{
	glEnableVertexAttribArrayARB(0);

	shader->bind();
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);

	//shader->setColor(Vector3f(1, 0, 0));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, 0, m_size_xy);

	//shader->setColor(Vector3f(0, 1, 0));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, m_size_xy, m_size_yz);

	//shader->setColor(Vector3f(0, 0, 1));
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), NULL);
	glDrawArrays(GL_LINE_LOOP, m_size_xy + m_size_yz, m_size_zx);

	glDisableVertexAttribArrayARB(0);
}