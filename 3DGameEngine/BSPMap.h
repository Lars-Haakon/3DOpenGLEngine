#ifndef BSPMAP_H
#define BSPMAP_H

#include "Vector3f.h"

struct BSPEntity
{
	unsigned char *entities;

	~BSPEntity()
	{
		delete[] entities;
	}
};

struct BSPTexture
{
	unsigned char name[64];
	int flags;
	int contents;
};

struct BSPPlane
{
	Vector3f normal;
	float distance;
};

struct BSPNode
{
	int plane;	// plane index
	int children[2]; // Children indices. Negative numbers are leaf indices: -(leaf+1)
	int mins[3]; // 	Integer bounding box min coord
	int maxs[3]; // 	Integer bounding box max coord
};

struct BSPLeaf
{
	int cluster;	// Visdata cluster index.
	int area;	// Areaportal area.
	int mins[3];	// Integer bounding box min coord.
	int maxs[3];	// Integer bounding box max coord.
	int leafface;	// First leafface for leaf.
	int n_leaffaces;	// Number of leaffaces for leaf.
	int leafbrush;	// First leafbrush for leaf.
	int n_leafbrushes;	// Number of leafbrushes for leaf.
};

struct BSPLeafFace
{
	int face; // face index
};

struct BSPLeafBrush
{
	int brush; // brush index
};

struct BSPModel
{
	Vector3f mins;	// Bounding box min coord.
	Vector3f maxs;	// Bounding box max coord.
	int face;	// First face for model.
	int n_faces;	// Number of faces for model.
	int brush;	// First brush for model.
	int n_brushes;	// Number of brushes for model.
};

struct BSPBrush
{
	int brushside;
	int n_brushsides;
	int texture;
};

struct BSPBrushside
{
	int plane; // plane index
	int texture; // texture index
};

struct BSPVertex
{
	Vector3f position;
	float texcoord[2][2];
	Vector3f normal;
	unsigned char color[4];
};

struct BSPMeshvert
{
	int offset; 	//Vertex index offset, relative to first vertex of corresponding face.
};

struct BSPEffect
{
	unsigned char name[64];
	int brush;
	int unknown;
};

struct BSPFace
{
	int texture;	//Texture index.
	int effect;	//Index into lump 12 (Effects), or - 1.
	int type;	//Face type. 1 = polygon, 2 = patch, 3 = mesh, 4 = billboard
	int vertex;	//Index of first vertex.
	int n_vertexes;	//Number of vertices.
	int meshvert;	//Index of first meshvert.
	int n_meshverts;	//Number of meshverts.
	int lm_index;	//Lightmap index.
	int lm_start[2];	//Corner of this face's lightmap image in lightmap.
	int lm_size[2];	//Size of this face's lightmap image in lightmap.
	Vector3f lm_origin;	//World space origin of lightmap.
	float lm_vecs[2][3];	//World space lightmap s and t unit vectors.
	Vector3f normal;	//Surface normal.
	int size[2];	//Patch dimensions.
};

struct BSPLightmap
{
	unsigned char map[128][128][3];	//Lightmap color data.RGB.
};

struct BSPLightvol
{
	unsigned char ambient[3];	//Ambient color component.RGB.
	unsigned char directional[3];	//Directional color component.RGB.
	unsigned char dir[2];	//Direction to light. 0 = phi, 1 = theta.
};

struct BSPVisdata
{
	int n_vecs;	//Number of vectors.
	int sz_vecs;	//Size of each vector, in bytes.
	unsigned char *vecs;	//Visibility data.One bit per cluster per vector.

	~BSPVisdata()
	{
		delete[] vecs;
	}
};

struct BSPDirentry
{
	int offset;
	int length;
};

struct BSPHeader
{
	unsigned char company[4];
	int version;
	BSPDirentry direntries[17];
};

// Loads with flipped xy
class BSPMap
{
private:
	int n_entitysize;
	BSPEntity *entity;
	int n_textures;
	BSPTexture *textures; 
	int n_planes;
	BSPPlane *planes;
	int n_nodes;
	BSPNode *nodes;
	int n_leafes;
	BSPLeaf *leafes;
	int n_leaffaces;
	BSPLeafFace *leaffaces;
	int n_leafbrushes;
	BSPLeafBrush *leafbrushes;
	int n_models;
	BSPModel *models;
	int n_brushes;
	BSPBrush *brushes;
	int n_brushsides;
	BSPBrushside *brushsides;
	int n_vertices;
	BSPVertex *vertices;
	int n_meshverts;
	BSPMeshvert *meshverts;
	int n_effects;
	BSPEffect *effects;
	int n_faces;
	BSPFace *faces;
	int n_lightmaps;
	BSPLightmap *lightmaps; 
	int n_lightvols;
	BSPLightvol *lightvols;
	int n_visdatasize;
	BSPVisdata *visdata;

	float readFloat(char *ptr);
	int readInt(char *ptr);
	short readShort(char *ptr);
	void readCharArray(unsigned char *dst, char *src, int size);
public:
	// Takes in a bytebuffer of the bsp file as parameter
	BSPMap(char *data);
	~BSPMap();

	int n_getTextures();
	BSPTexture *getTextures();
	int n_getPlanes();
	BSPPlane *getPlanes();
	int n_getNodes();
	BSPNode *getNodes();
	int n_getLeafes();
	BSPLeaf *getLeafes();
	int n_getVertices();
	BSPVertex *getVertices();
	int n_getMeshverts();
	BSPMeshvert *getMeshverts();
	int n_getFaces();
	BSPFace *getFaces();
	int n_getLightmaps();
	BSPLightmap *getLightmaps();
};

#endif