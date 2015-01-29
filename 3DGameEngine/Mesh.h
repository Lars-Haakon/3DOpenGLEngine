#ifndef MESH_H
#define MESH_H

#include "BoundingVolume.h"
#include "Matrix4f.h"
#include "Geometry.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addVertices(Vertex *vertices, int *indices, int vertSize, int indSize, bool calcNormals);
	void setVertices(Vertex *vertices, int vertSize);
	void draw();
protected:
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
	int m_size;

	void calcNormals(Vertex *vertices, int *inices, int vertSize, int indSize);
	void calcTangents(Vertex *vertices, int *inices, int vertSize, int indSize);

};

#endif