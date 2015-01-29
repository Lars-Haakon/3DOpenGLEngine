#include "BSPMap.h"
#include "Console.h"

BSPMap::BSPMap(char *data)
{
	/*---------READ HEADER------------*/
	BSPHeader header;

	readCharArray(header.company, data, 4);
	header.version = readInt(data + sizeof(int));

	for (int i = 0; i < 17; i++)
	{
		header.direntries[i].offset = readInt(data + (2 + 2 * i) * sizeof(int));
		header.direntries[i].length = readInt(data + (3 + 2 * i) * sizeof(int));
	}
	/*----------DONE READING HEADER-------------*/

	/*--------READ ENTITIES----------*/
	int entitiesOffset = header.direntries[0].offset;
	int entitiesLength = header.direntries[0].length;
	n_entitysize = entitiesLength;
	entity = new BSPEntity;
	entity->entities = new unsigned char[entitiesLength];
	readCharArray(entity->entities, data + entitiesOffset, entitiesLength);
	/*----------DONE READING ENTITIES*/

	/*----------------READ TEXTURES----------------------*/
	int texturesOffset = header.direntries[1].offset;
	int texturesLength = header.direntries[1].length;
	n_textures = texturesLength / sizeof(BSPTexture);
	textures = new BSPTexture[n_textures];
	for (int i = 0; i < n_textures; i++)
	{
		readCharArray(textures[i].name, data + texturesOffset + i*sizeof(BSPTexture), 64);
		textures[i].flags = readInt(data + texturesOffset + i*sizeof(BSPTexture) + 64);
		textures[i].contents = readInt(data + texturesOffset + i*sizeof(BSPTexture)+68);
	}
	/*------------DONE READING TEXTURES-----------------*/

	/*---------READ PLANES------------*/
	int planesOffset = header.direntries[2].offset;
	int planesLength = header.direntries[2].length;
	n_planes = planesLength / sizeof(BSPPlane);
	planes = new BSPPlane[n_planes];
	for (int i = 0; i < n_planes; i++)
	{
		float x = readFloat(data + planesOffset + i*sizeof(BSPPlane) + 0*sizeof(float));
		float y = readFloat(data + planesOffset + i*sizeof(BSPPlane) + 1*sizeof(float));
		float z = readFloat(data + planesOffset + i*sizeof(BSPPlane) + 2*sizeof(float));
		float dist = readFloat(data + planesOffset + i*sizeof(BSPPlane) + 3*sizeof(float));

		planes[i].normal = Vector3f(x, z, -y);
		planes[i].distance = dist;
	}
	/*----------DONE READING PLANES-------------*/

	/*--------READ NODES----------*/
	int nodesOffset = header.direntries[3].offset;
	int nodesLength = header.direntries[3].length;
	n_nodes = nodesLength / sizeof(BSPNode);
	nodes = new BSPNode[n_nodes];
	for (int i = 0; i < n_nodes; i++)
	{
		nodes[i].plane = readInt(data + nodesOffset + i*sizeof(BSPNode)+0*sizeof(int));
		nodes[i].children[0] = readInt(data + nodesOffset + i*sizeof(BSPNode)+1 * sizeof(int));
		nodes[i].children[1] = readInt(data + nodesOffset + i*sizeof(BSPNode)+2 * sizeof(int));
		nodes[i].mins[0] = readInt(data + nodesOffset + i*sizeof(BSPNode)+3 * sizeof(int));
		nodes[i].mins[1] = readInt(data + nodesOffset + i*sizeof(BSPNode)+4 * sizeof(int));
		nodes[i].mins[2] = readInt(data + nodesOffset + i*sizeof(BSPNode)+5 * sizeof(int));
		nodes[i].maxs[0] = readInt(data + nodesOffset + i*sizeof(BSPNode)+6 * sizeof(int));
		nodes[i].maxs[1] = readInt(data + nodesOffset + i*sizeof(BSPNode)+7 * sizeof(int));
		nodes[i].maxs[2] = readInt(data + nodesOffset + i*sizeof(BSPNode)+8 * sizeof(int));
	}
	/*-------------DONE READING NODES-----------------*/

	/*-----------READ LEAFES--------------*/
	int leafesOffset = header.direntries[4].offset;
	int leafesLength = header.direntries[4].length;
	n_leafes = leafesLength / sizeof(BSPLeaf);
	leafes = new BSPLeaf[n_leafes];
	for (int i = 0; i < n_leafes; i++)
	{
		leafes[i].cluster = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+0 * sizeof(int));
		leafes[i].area = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+1 * sizeof(int));
		leafes[i].mins[0] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+2 * sizeof(int));
		leafes[i].mins[1] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+3 * sizeof(int));
		leafes[i].mins[2] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+4 * sizeof(int));
		leafes[i].maxs[0] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+5 * sizeof(int));
		leafes[i].maxs[1] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+6 * sizeof(int));
		leafes[i].maxs[2] = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+7 * sizeof(int));
		leafes[i].leafface = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+8 * sizeof(int));
		leafes[i].n_leaffaces = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+9 * sizeof(int));
		leafes[i].leafbrush = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+10 * sizeof(int));
		leafes[i].n_leafbrushes = readInt(data + leafesOffset + i*sizeof(BSPLeaf)+11 * sizeof(int));
	}
	/*-----------------DONE READING LEAFES----------------*/

	/*-------------READ LEAFFACES----------------*/
	int leaffacesOffset = header.direntries[5].offset;
	int leaffacesLength = header.direntries[5].length;
	n_leaffaces = leaffacesLength / sizeof(BSPLeafFace);
	leaffaces = new BSPLeafFace[n_leaffaces];
	for (int i = 0; i < n_leaffaces; i++)
	{
		leaffaces[i].face = readInt(data + leaffacesOffset + i*sizeof(BSPLeafFace));
	}
	/*--------DONE READING LEAFFACES--------------*/

	/*-----------READ LEAFBRUSHES--------------*/
	int leafbrushesOffset = header.direntries[6].offset;
	int leafbrushesLength = header.direntries[6].length;
	n_leafbrushes = leafbrushesLength / sizeof(BSPLeafBrush);
	leafbrushes = new BSPLeafBrush[n_leafbrushes];
	for (int i = 0; i < n_leafbrushes; i++)
	{
		leafbrushes[i].brush = readInt(data + leafbrushesOffset + i*sizeof(BSPLeafBrush));
	}
	/*----------DONE READING LEAFBRUSHES---------------*/

	/*-----------READ MODELS----------------*/
	int modelsOffset = header.direntries[7].offset;
	int modelsLength = header.direntries[7].length;
	n_models = modelsLength / sizeof(BSPModel);
	models = new BSPModel[n_models];
	for (int i = 0; i < n_models; i++)
	{
		float minx = readFloat(data + modelsOffset + i*sizeof(BSPModel)+0 * sizeof(float));
		float miny = readFloat(data + modelsOffset + i*sizeof(BSPModel)+1 * sizeof(float));
		float minz = readFloat(data + modelsOffset + i*sizeof(BSPModel)+2 * sizeof(float));
		models[i].mins = Vector3f(minx, minz, -miny);

		float maxx = readFloat(data + modelsOffset + i*sizeof(BSPModel)+3 * sizeof(float));
		float maxy = readFloat(data + modelsOffset + i*sizeof(BSPModel)+4 * sizeof(float));
		float maxz = readFloat(data + modelsOffset + i*sizeof(BSPModel)+5 * sizeof(float));
		models[i].maxs = Vector3f(maxx, maxz, -maxy);

		models[i].face = readInt(data + modelsOffset + i*sizeof(BSPModel)+6 * sizeof(float));
		models[i].n_faces = readInt(data + modelsOffset + i*sizeof(BSPModel)+6 * sizeof(float) + 1*sizeof(int));
		models[i].brush = readInt(data + modelsOffset + i*sizeof(BSPModel)+6 * sizeof(float)+ 2*sizeof(int));
		models[i].n_brushes = readInt(data + modelsOffset + i*sizeof(BSPModel)+6 * sizeof(float)+3*sizeof(int));
	}
	/*---------------DONE READING MODELS--------------*/

	/*-----------READ BRUSHES---------------*/
	int brushesOffset = header.direntries[8].offset;
	int brushesLength = header.direntries[8].length;
	n_brushes = brushesLength / sizeof(BSPBrush);
	brushes = new BSPBrush[n_brushes];
	for (int i = 0; i < n_brushes; i++)
	{
		brushes[i].brushside = readInt(data + brushesOffset + i*sizeof(BSPBrush) + 0*sizeof(int));
		brushes[i].n_brushsides = readInt(data + brushesOffset + i*sizeof(BSPBrush)+1 * sizeof(int));
		brushes[i].texture = readInt(data + brushesOffset + i*sizeof(BSPBrush)+2*sizeof(int));
	}
	/*-------------DONE READING BRUSHES-------------*/

	/*----------READ BRUSHSIDES-----------*/
	int brushsidesOffset = header.direntries[9].offset;
	int brushsidesLength = header.direntries[9].length; 
	n_brushsides = brushsidesLength / sizeof(BSPBrushside);
	brushsides = new BSPBrushside[n_brushsides];
	for (int i = 0; i < n_brushsides; i++)
	{
		brushsides[i].plane = readInt(data + brushsidesOffset + i*sizeof(BSPBrushside) + 0*sizeof(int));
		brushsides[i].texture = readInt(data + brushsidesOffset + i*sizeof(BSPBrushside)+1 * sizeof(int));
	}
	/*----------DONE READING BRUSHSIDES-----------*/

	/*---------READ VERTICES---------*/
	int verticesOffset = header.direntries[10].offset;
	int verticesLength = header.direntries[10].length;
	n_vertices = verticesLength / sizeof(BSPVertex);
	vertices = new BSPVertex[n_vertices];
	for (int i = 0; i < n_vertices; i++)
	{
		float x = readFloat(data + verticesOffset + i*sizeof(BSPVertex) + 0*sizeof(float));
		float y = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+1 * sizeof(float));
		float z = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+2 * sizeof(float));
		vertices[i].position = Vector3f(x, z, -y);

		vertices[i].texcoord[0][0] = readFloat(data + verticesOffset + i*sizeof(BSPVertex) + 3*sizeof(float));
		vertices[i].texcoord[0][1] = -readFloat(data + verticesOffset + i*sizeof(BSPVertex)+4 * sizeof(float));
		vertices[i].texcoord[1][0] = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+5 * sizeof(float));
		vertices[i].texcoord[1][1] = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+6 * sizeof(float));

		float nx = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+ 7 * sizeof(float));
		float ny = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+ 8 * sizeof(float));
		float nz = readFloat(data + verticesOffset + i*sizeof(BSPVertex)+ 9 * sizeof(float));
		vertices[i].normal = Vector3f(nx, nz, -ny);

		readCharArray(vertices[i].color, data + verticesOffset + i*sizeof(BSPVertex)+10 * sizeof(float), 4);
	}
	/*--------DONE READING VERTICES-----------*/

	/*----------READ MESHVERTS-----------*/
	int meshvertsOffset = header.direntries[11].offset;
	int meshvertLength = header.direntries[11].length;
	n_meshverts = meshvertLength / sizeof(BSPMeshvert);
	meshverts = new BSPMeshvert[n_meshverts];
	for (int i = 0; i < n_meshverts / 3; i++)
	{
		meshverts[3*i+2].offset = readInt(data + meshvertsOffset + i*3*sizeof(BSPMeshvert));
		meshverts[3*i+1].offset = readInt(data + meshvertsOffset + i*3*sizeof(BSPMeshvert) + sizeof(int));
		meshverts[3*i].offset = readInt(data + meshvertsOffset + i*3*sizeof(BSPMeshvert) + 2*sizeof(int));
	}
	/*---------DONE READING MESHVERTS------------*/

	/*--------READ EFFECTS------------*/
	int effectsOffset = header.direntries[12].offset;
	int effectsLength = header.direntries[12].length;
	n_effects = effectsLength / sizeof(BSPEffect);
	effects = new BSPEffect[n_effects];
	for (int i = 0; i < n_effects; i++)
	{
		readCharArray(effects[i].name, data + effectsOffset + i*sizeof(BSPEffect), 64);
		effects[i].brush = readInt(data + effectsOffset + i*sizeof(BSPEffect)+64);
		effects[i].unknown = readInt(data + effectsOffset + i*sizeof(BSPEffect)+68);
	}
	/*----------DONE READING EFFECTS-------------*/

	/*--------READ FACES------------*/
	int facesOffset = header.direntries[13].offset;
	int facesLength = header.direntries[13].length;
	n_faces = facesLength / sizeof(BSPFace);
	faces = new BSPFace[n_faces];
	for (int i = 0; i < n_faces; i++)
	{
		faces[i].texture = readInt(data + facesOffset + i*sizeof(BSPFace) + 0*sizeof(int));
		faces[i].effect = readInt(data + facesOffset + i*sizeof(BSPFace)+1 * sizeof(int));
		faces[i].type = readInt(data + facesOffset + i*sizeof(BSPFace)+2 * sizeof(int));
		faces[i].vertex = readInt(data + facesOffset + i*sizeof(BSPFace)+3 * sizeof(int));
		faces[i].n_vertexes = readInt(data + facesOffset + i*sizeof(BSPFace)+4 * sizeof(int));
		faces[i].meshvert = readInt(data + facesOffset + i*sizeof(BSPFace)+5 * sizeof(int));
		faces[i].n_meshverts = readInt(data + facesOffset + i*sizeof(BSPFace)+6 * sizeof(int));
		faces[i].lm_index = readInt(data + facesOffset + i*sizeof(BSPFace)+7 * sizeof(int));
		faces[i].lm_start[0] = readInt(data + facesOffset + i*sizeof(BSPFace)+8 * sizeof(int));
		faces[i].lm_start[1] = readInt(data + facesOffset + i*sizeof(BSPFace)+9 * sizeof(int));
		faces[i].lm_size[0] = readInt(data + facesOffset + i*sizeof(BSPFace)+10 * sizeof(int));
		faces[i].lm_size[1] = readInt(data + facesOffset + i*sizeof(BSPFace)+11* sizeof(int));

		float x = readFloat(data + facesOffset + i*sizeof(BSPFace) + 12*sizeof(int) + 0*sizeof(float));
		float y = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+1 * sizeof(float));
		float z = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+2 * sizeof(float));
		faces[i].lm_origin = Vector3f(x, z, -y);

		faces[i].lm_vecs[0][0] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+3 * sizeof(float));
		faces[i].lm_vecs[0][1] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+4 * sizeof(float));
		faces[i].lm_vecs[0][2] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+5 * sizeof(float));
		faces[i].lm_vecs[1][0] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+6 * sizeof(float));
		faces[i].lm_vecs[1][1] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+7 * sizeof(float));
		faces[i].lm_vecs[1][2] = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+8 * sizeof(float));

		float nx = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+9 * sizeof(float));
		float ny = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+10 * sizeof(float));
		float nz = readFloat(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+11 * sizeof(float));
		faces[i].normal = Vector3f(nx, nz, -ny);

		faces[i].size[0] = readInt(data + facesOffset + i*sizeof(BSPFace)+12 * sizeof(int)+12 * sizeof(float));
		faces[i].size[1] = readInt(data + facesOffset + i*sizeof(BSPFace)+13 * sizeof(int)+12 * sizeof(float));
	}
	/*-----------DONE READING FACES----------*/

	/*---------READ LIGHTMAPS---------*/
	int lightmapsOffset = header.direntries[14].offset;
	int lightmapsLength = header.direntries[14].length;
	n_lightmaps = lightmapsLength / sizeof(BSPLightmap);
	lightmaps = new BSPLightmap[n_lightmaps];
	for (int i = 0; i < n_lightmaps; i++)
	{
		readCharArray((unsigned char*)lightmaps[i].map, data + lightmapsOffset + i*sizeof(BSPLightmap), sizeof(BSPLightmap));
	}
	/*--------DONE READING LIGHTMAPS---------------*/

	/*--------READ LIGHTVOLS---------*/
	int lightvolsOffset = header.direntries[15].offset;
	int lightvolsLength = header.direntries[15].length;
	n_lightvols = lightvolsLength / sizeof(BSPLightvol);
	lightvols = new BSPLightvol[n_lightvols];
	for (int i = 0; i < n_lightvols; i++)
	{
		readCharArray(lightvols[i].ambient, data + lightvolsOffset + i*sizeof(BSPLightvol), 3);
		readCharArray(lightvols[i].directional, data + lightvolsOffset + i*sizeof(BSPLightvol)+3, 3);
		readCharArray(lightvols[i].dir, data + lightvolsOffset + i*sizeof(BSPLightvol)+6, 2);
	}
	/*-------DONE READING LIGHTVOLS-----------*/

	/*---------READ VISDATA------------*/
	int visdataOffset = header.direntries[16].offset;
	int visdataLength = header.direntries[16].length;
	n_visdatasize = visdataLength;
	if (visdataLength != 0)
	{
		visdata = new BSPVisdata;
		visdata->n_vecs = readInt(data + visdataOffset);
		visdata->sz_vecs = readInt(data + visdataOffset + sizeof(int));
		visdata->vecs = new unsigned char[visdata->n_vecs * visdata->sz_vecs];
		readCharArray(visdata->vecs, data + visdataOffset + 2 * sizeof(int), visdataLength - 2 * sizeof(int));
	}
	/*-----------DONE READING VISDATA-------------*/
}

BSPMap::~BSPMap()
{
	delete entity;
	delete[] textures;
	delete[] planes;
	delete[] nodes;
	delete[] leafes;
	delete[] leaffaces;
	delete[] leafbrushes;
	delete[] models;
	delete[] brushes;
	delete[] brushsides;
	delete[] vertices;
	delete[] meshverts;
	delete[] effects;
	delete[] faces;
	delete[] lightmaps;
	delete[] lightvols;
	delete visdata;
}

float BSPMap::readFloat(char *ptr) // convert between big-endian and little-endian
{
	return *((float*)ptr);
}

int BSPMap::readInt(char *ptr) // convert between big-endian and little-endian
{
	return (ptr[0] & 0xFF) | ((ptr[1] << 8) & 0xFF00) | ((ptr[2] << 16) & 0xFF0000) | ((ptr[3] << 24) & 0xFF000000);
}

short BSPMap::readShort(char *ptr) // convert between big-endian and little-endian
{
	return (ptr[0] & 0xFF) | ((ptr[1] << 8) & 0xFF00);
}

void BSPMap::readCharArray(unsigned char *dst, char *src, int size)
{
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
}

int BSPMap::n_getTextures()
{
	return n_textures;
}
BSPTexture *BSPMap::getTextures()
{
	return textures;
}
int BSPMap::n_getPlanes()
{
	return n_planes;
}
BSPPlane *BSPMap::getPlanes()
{
	return planes;
}
int BSPMap::n_getNodes()
{
	return n_nodes;
}
BSPNode *BSPMap::getNodes()
{
	return nodes;
}
int BSPMap::n_getLeafes()
{
	return n_leafes;
}
BSPLeaf *BSPMap::getLeafes()
{
	return leafes;
}
int BSPMap::n_getVertices()
{
	return n_vertices;
}
BSPVertex *BSPMap::getVertices()
{
	return vertices;
}
int BSPMap::n_getMeshverts()
{
	return n_meshverts;
}
BSPMeshvert *BSPMap::getMeshverts()
{
	return meshverts;
}
int BSPMap::n_getFaces()
{
	return n_faces;
}
BSPFace *BSPMap::getFaces()
{
	return faces;
}
int BSPMap::n_getLightmaps()
{
	return n_lightmaps;
}
BSPLightmap *BSPMap::getLightmaps()
{
	return lightmaps;
}