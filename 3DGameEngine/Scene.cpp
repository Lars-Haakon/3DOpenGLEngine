#include "Scene.h"
#include "ForwardAmbient.h"
#include "ForwardDirectional.h"
#include "ForwardPoint.h"
#include "ForwardSpot.h"
#include <stdio.h>
#include "Console.h"
#include "GLExtFunc.h"
#include <math.h>

Scene::Scene(char *sceneName)
{
	char *data = NULL;
	FILE *fp = NULL;
	fopen_s(&fp, sceneName, "rb");
	BSPMap *map;

	if (fp != NULL)
	{
		if (fseek(fp, 0L, SEEK_END) == 0)
		{
			/* Get the size of the file. */
			int bufsize = ftell(fp);
			rewind(fp);
			/* Allocate our buffer to that size. */
			data = new char[bufsize];

			/* Read the entire file into memory. */
			int newLen = fread(data, 1, bufsize, fp);
			fclose(fp);
			if (newLen == 0)
			{
				exit(1);
			}

			map = new BSPMap(data);
			delete[] data;
		}
		else
			exit(1);
	}
	else
		exit(1);

	for (int i = 0; i < map->n_getTextures(); i++)
	{
		unsigned char *textureName = map->getTextures()[i].name;

		if (strcmp((char*)textureName, "textures/common/caulk") != 0)
		{
			FILE *materialFile = NULL;
			fopen_s(&materialFile, (const char*)textureName, "r");

			if (materialFile != NULL)
			{
				char colorName[128];
				fscanf_s(materialFile, "colorMap %s\r\n", colorName, 128);
				char normalName[128];
				fscanf_s(materialFile, "normalMap %s\r\n", normalName, 128);
				float specularIntensity;
				fscanf_s(materialFile, "specularIntensity %f\r\n", &specularIntensity);
				float specularPower;
				fscanf_s(materialFile, "specularPower %f\r\n", &specularPower);

				materials.add(new Material(Texture::loadBMP(colorName), Texture::loadBMP(normalName), specularIntensity, specularPower));
			}
			else
				exit(1);	

			fclose(materialFile);
		}
		else
			materials.add(NULL);
	}

	LinkedList<Vertex> *vertices = new LinkedList<Vertex>[materials.getLength()];
	LinkedList<int> *indices = new LinkedList<int>[materials.getLength()];

	int numberOfFaces = map->n_getFaces();
	for (int i = 0; i < numberOfFaces; i++)
	{
		BSPFace f = map->getFaces()[i];
		int textureID = f.texture;

		// append more vertices and indices
		int numberOfIndices = f.n_meshverts;
		for (int j = 0; j < numberOfIndices; j++)
		{
			int *ind = new int;
			*ind = map->getMeshverts()[f.meshvert + j].offset + vertices[textureID].getLength();
			indices[textureID].add(ind);
		}
		
		int numberOfVertices = f.n_vertexes;
		for (int j = 0; j < numberOfVertices; j++)
		{
			BSPVertex v = map->getVertices()[f.vertex + j];
			Vertex *vertex = new Vertex();
			vertex->pos = v.position;
			vertex->normal = v.normal;
			vertex->texCoord = Vector2f(v.texcoord[0][0], v.texcoord[0][1]);
			vertices[textureID].add(vertex);
		}
	}

	for (int i = 0; i < materials.getLength(); i++)
	{
		if (materials.get(i) != NULL)
		{
			Mesh *mesh = new Mesh();
			Vertex *vA = vertices[i].toArray();
			int *iA = indices[i].toArray();

			mesh->addVertices(vA, iA, vertices[i].getLength(), indices[i].getLength(), false);
			delete[] vA;
			delete[] iA;

			meshes.add(mesh);
		}
		else
			meshes.add(NULL);	
	}

	delete[] vertices;
	delete[] indices;

	ambientShader = new ForwardAmbient();
	directionalShader = new ForwardDirectional();
	pointShader = new ForwardPoint();
	spotShader = new ForwardSpot();
	delete map;
}

Scene::~Scene()
{
	delete ambientShader;
	delete directionalShader;
	delete pointShader;
	delete spotShader;
}

void Scene::update()
{

}

void Scene::render()
{
	Transform transform;

	ambientShader->bind();
	for (int i = 0; i < meshes.getLength(); i++)
	{
		if (meshes.get(i) == NULL) // CAULK
			continue;

		ambientShader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), materials.get(i));
		meshes.get(i)->draw();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	/*directionalShader->bind();
	for (int i = 0; i < meshes.getLength(); i++)
	{
		if (meshes.get(i) == NULL) // CAULK
			continue;

		directionalShader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), materials.get(i));
		meshes.get(i)->draw();
	}*/

	pointShader->bind();
	for (int i = 0; i < meshes.getLength(); i++)
	{
		if (meshes.get(i) == NULL) // CAULK
			continue;

		pointShader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), materials.get(i));
		meshes.get(i)->draw();
	}

	/*spotShader->bind();
	for (int i = 0; i < meshes.getLength(); i++)
	{
		if (meshes.get(i) == NULL) // CAULK
			continue;

		spotShader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), materials.get(i));
		meshes.get(i)->draw();
	}*/
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}