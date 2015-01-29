#ifndef ANIMATEDBODY_H
#define ANIMATEDBODY_H

#include "Geometry.h"
#include "BoundingVolume.h"
#include <stdio.h>

//Measuring programming progress by lines of code is like measuring aircraft building progress by weight - Bill Gates

struct Bone
{
	char name[20]; // used as identification

	int numChildren;
	Bone **children; // pointer to array of children pointers
	Bone *father;

	Matrix4f frameTransformMatrix;
	Matrix4f inverseBindMatrix;

	int numVertexIndices;
	int *vertexIndices;
	float *weights;
	Matrix4f sW; // Skin Matrix

	int numFrames;
	Matrix4f *frameMatrices; // array of the frame matrices

	int currentChild; // used for iteration between the bones
};

class AnimatedBody
{
public:
	AnimatedBody();
	~AnimatedBody();

	void render();
	void setVertices(int v_size, Vector3f *vertices);
	void setIndices(int i_size, int *indices);
	void setRootBone(Bone *rootBone);
	void addSkinWeights(char *boneName, int numVert, int *vertexIndices, float *weights, Matrix4f &sW);
	void addBoneAnimation(char *boneName, int numFrames, Matrix4f *frameMatrices);

	void setAnimTicksPerSecond(int animTicksPerSecond);

	static AnimatedBody *loadAO(char *filename);
private:
	static void processRootMatrix(FILE *stream, AnimatedBody *object);
	static void processBoneStructure(FILE *stream, AnimatedBody *object);
	static void processMesh(FILE *stream, AnimatedBody *object);
	static void processSkinWeight(FILE *stream, AnimatedBody *object);
	static void processAnimTicksPerSecond(FILE *stream, AnimatedBody *object);
	static void processAnimation(FILE *stream, AnimatedBody *object);

	static void removeSpaces(char *line);
	static bool find(char *line, char *c);
	static Matrix4f cM(Bone *bone);
	static Matrix4f cF(Bone *bone, int frame);

	//Recursive traversal functions using post ordering
	void calcVertices(Bone *bPtr, Vector3f *drawVertices, int frame);
	void freeBoneStructure(Bone *bPtr);

	unsigned int m_vbo;
	unsigned int m_ibo;
	int m_size;
	ForwardAmbient *shader;
	Material *mat;

	BoundingVolume *boundingVolume;
	Bone *rootBone;
	Matrix4f rootMatrix;

	int numVertices;
	Vector3f *bindPosVertices;
	int numIndices;
	int *indices;

	int animTicksPerSecond;
	double referenceTime;
};

#endif