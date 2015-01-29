#include "Mesh.h"
#include "GLExtFunc.h"
#include <math.h>
#include <fstream>
#include "Console.h"

Mesh::Mesh()
{
	glGenBuffersARB(1, &m_vbo);
	glGenBuffersARB(1, &m_ibo);
	m_size = 0;
}

Mesh::~Mesh()
{
	glDeleteBuffersARB(1, &m_vbo);
	glDeleteBuffersARB(1, &m_ibo);
}

void Mesh::addVertices(Vertex *vertices, int *indices, int vertSize, int indSize, bool calcNormals)
{
	m_size = indSize;

	if (calcNormals)
		this->calcNormals(vertices, indices, vertSize, indSize);

	this->calcTangents(vertices, indices, vertSize, indSize);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_ibo);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indSize * sizeof(int), indices, GL_STATIC_DRAW_ARB);
}

void Mesh::setVertices(Vertex *vertices, int vertSize)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW_ARB);
}

void Mesh::draw()
{
	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
	glEnableVertexAttribArrayARB(2);
	glEnableVertexAttribArrayARB(3);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(Vector3f));
	glVertexAttribPointerARB(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(2*sizeof(Vector3f)));
	glVertexAttribPointerARB(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3*sizeof(Vector3f)));

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_ibo);
	glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArrayARB(3);
	glDisableVertexAttribArrayARB(2);
	glDisableVertexAttribArrayARB(1);
	glDisableVertexAttribArrayARB(0);
}

void Mesh::calcNormals(Vertex *vertices, int *indices, int vertSize, int indSize)
{
	for (int i = 0; i < indSize; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3f v1 = vertices[i1].pos - vertices[i0].pos;
		Vector3f v2 = vertices[i2].pos - vertices[i0].pos;

		Vector3f normal = v1.cross(v2).normalized();

		//vertices[i0].normal = (vertices[i0].normal + normal).normalized();
		//vertices[i1].normal = (vertices[i1].normal + normal).normalized();
		//vertices[i2].normal = (vertices[i2].normal + normal).normalized();
	}
}

void Mesh::calcTangents(Vertex *vertices, int *indices, int vertSize, int indSize)
{
	for (int i = 0; i < indSize; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3f edge1 = vertices[i1].pos - vertices[i0].pos;
		Vector3f edge2 = vertices[i2].pos - vertices[i0].pos;

		float deltaU1 = vertices[i1].texCoord.getX() - vertices[i0].texCoord.getX();
		float deltaU2 = vertices[i2].texCoord.getX() - vertices[i0].texCoord.getX();
		float deltaV1 = vertices[i1].texCoord.getY() - vertices[i0].texCoord.getY();
		float deltaV2 = vertices[i2].texCoord.getY() - vertices[i0].texCoord.getY();

		float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

		Vector3f tangent = Vector3f(f * (deltaV2 * edge1.getX() - deltaV1 * edge2.getX()), 
									f * (deltaV2 * edge1.getY() - deltaV1 * edge2.getY()), 
									f * (deltaV2 * edge1.getZ() - deltaV1 * edge2.getZ()));

		//Bitangent example, in Java
		//		Vector3f bitangent = new Vector3f(0,0,0);
		//		
		//		bitangent.setX(f * (-deltaU2 * edge1.getX() - deltaU1 * edge2.getX()));
		//		bitangent.setX(f * (-deltaU2 * edge1.getY() - deltaU1 * edge2.getY()));
		//		bitangent.setX(f * (-deltaU2 * edge1.getZ() - deltaU1 * edge2.getZ()));

		vertices[i0].tangent = (vertices[i0].tangent + tangent).normalized();
		vertices[i1].tangent = (vertices[i1].tangent + tangent).normalized();
		vertices[i2].tangent = (vertices[i2].tangent + tangent).normalized();
	}
}