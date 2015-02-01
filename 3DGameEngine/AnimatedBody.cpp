#include "AnimatedBody.h"
#include "Console.h"
#include "GLExtFunc.h"
#include "Time.h"

AnimatedBody::AnimatedBody()
{
	glGenBuffersARB(1, &m_vbo);
	glGenBuffersARB(1, &m_ibo);
	m_size = 0;

	shader = new ForwardAmbient();
	mat = new Material(Texture::loadBMP("textures/mp5.bmp"), Texture::loadBMP("textures/mp5.bmp"), 0.4f, 1.6f);
	//shader->setColor(Vector3f(1, 0, 1));

	referenceTime = Time::getTime();
}

AnimatedBody::~AnimatedBody()
{
	delete[] bindPosVertices;
	delete[] indices;

	glDeleteBuffersARB(1, &m_vbo);
	glDeleteBuffersARB(1, &m_ibo);
	delete shader;
	delete mat;

	freeBoneStructure(rootBone);
}

Matrix4f global;

void AnimatedBody::render()
{
	double elapsed = Time::getTime() - referenceTime;
	double frameInAnimation = animTicksPerSecond * elapsed;
	int frame = (int) frameInAnimation % 120; // 120 is the number of frames in the animation

	Vector3f *drawVertices = new Vector3f[numVertices];
	calcVertices(rootBone, drawVertices, frame);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, numVertices * sizeof(Vector3f), drawVertices, GL_STATIC_DRAW_ARB);
	free(drawVertices);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_ibo);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, numIndices * sizeof(int), indices, GL_STATIC_DRAW_ARB);

	Transform transform;

	shader->bind();
	shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(), mat);

	glEnableVertexAttribArrayARB(0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_ibo);
	glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArrayARB(0);
}

void AnimatedBody::setVertices(int v_size, Vector3f *vertices)
{
	this->numVertices = v_size;
	this->bindPosVertices = vertices;
}

void AnimatedBody::setIndices(int i_size, int *indices)
{
	this->numIndices = i_size;
	this->indices = indices;
}

void AnimatedBody::setAnimTicksPerSecond(int animTicksPerSecond)
{
	this->animTicksPerSecond = animTicksPerSecond;
}

void AnimatedBody::setRootBone(Bone *rootBone)
{
	this->rootBone = rootBone;
}

void AnimatedBody::calcVertices(Bone *bPtr, Vector3f *drawVertices, int frame)
{
	for (int i = 0; i < bPtr->numChildren; i++)
	{
		calcVertices(bPtr->children[i], drawVertices, frame);
	}

	if (bPtr->father != NULL) // if its not the root bone
	{
		Matrix4f finalMatrix = sW(bPtr) *cF(bPtr, frame);
		for (int i = 0; i < bPtr->numVertexIndices; i++)
		{
			drawVertices[bPtr->vertexIndices[i]] += (finalMatrix*bPtr->inverseBindMatrix * bindPosVertices[bPtr->vertexIndices[i]]) * bPtr->weights[i];
		}
	}
}

//recursive function to free all memory occupied by the bone structure
void AnimatedBody::freeBoneStructure(Bone *bPtr)
{
	for (int i = 0; i < bPtr->numChildren; i++)
	{
		freeBoneStructure(bPtr->children[i]);
	}

	delete[] bPtr->children;
	delete bPtr;
}

void AnimatedBody::addSkinWeights(char *boneName, int numVert, int *ind, float *weights, Matrix4f &sW)
{
	Bone *ptr = rootBone;
	do
	{
		if (ptr->currentChild == ptr->numChildren)
		{
			if (strcmp(ptr->name, boneName) == 0)
			{
				ptr->numVertexIndices = numVert;
				ptr->vertexIndices = ind;
				ptr->weights = weights;
				ptr->sW = sW.transposed();
				ptr->currentChild = 0;

				while (ptr->father != NULL)
				{
					ptr->father->currentChild = 0;
					ptr = ptr->father;
				}
				break;
			}

			ptr->currentChild = 0;
			if (ptr->father == NULL)
				break;

			ptr = ptr->father;
		}
		else
		{
			ptr = ptr->children[ptr->currentChild];
			ptr->father->currentChild++;
		}
	} while (ptr != NULL);
}

void AnimatedBody::addBoneAnimation(char *boneName, int numFrames, Matrix4f *frameMatrices)
{
	Bone *ptr = rootBone;
	do
	{
		if (ptr->currentChild == ptr->numChildren)
		{
			if (strcmp(ptr->name, boneName) == 0)
			{
				ptr->numFrames = numFrames;
				ptr->frameMatrices = frameMatrices;
				ptr->currentChild = 0;

				while (ptr->father != NULL)
				{
					ptr->father->currentChild = 0;
					ptr = ptr->father;
				}
				break;
			}

			ptr->currentChild = 0;
			if (ptr->father == NULL)
				break;

			ptr = ptr->father;
		}
		else
		{
			ptr = ptr->children[ptr->currentChild];
			ptr->father->currentChild++;
		}
	} while (ptr != NULL);
}

Matrix4f AnimatedBody::cM(Bone *bone)
{
	if (bone->father->father != NULL)
		return bone->frameTransformMatrix * cM(bone->father);
	
	return bone->frameTransformMatrix;
}

Matrix4f AnimatedBody::sW(Bone *bone)
{
	if (bone->father->father != NULL)
		return bone->sW * sW(bone->father);

	return bone->sW;
}

Matrix4f AnimatedBody::cF(Bone *bone, int frame)
{
	if (bone->father->father != NULL)
		return bone->frameMatrices[frame] * cF(bone->father, frame);
	
	return bone->frameMatrices[frame];
}

AnimatedBody *AnimatedBody::loadAO(char *filename)
{
	/*1.000000, 0.000000, 0.000000, 0.000000,
	0.000000, -0.000000, -1.000000, 0.000000,
	0.000000, 1.000000, -0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 1.000000;;*/

	global[0][0] = 1;	global[0][1] = 0;	global[0][2] = 0;	global[0][3] = 0;
	global[1][0] = 0;	global[1][1] = 0;	global[1][2] = 1;	global[1][3] = 0;
	global[2][0] = 0;	global[2][1] = -1;	global[2][2] = 0;	global[2][3] = 0;
	global[3][0] = 0;	global[3][1] = 0;	global[3][2] = 0;	global[3][3] = 1;

	FILE *pFile = NULL;
	fopen_s(&pFile, filename, "r");

	if (pFile == NULL)
	{
		Console::printf("%s\n", "Could not open file");
		exit(1);
	}

	AnimatedBody *object = new AnimatedBody();

	while (!feof(pFile))
	{
		char line[80];
		fgets(line, 80, pFile);
		removeSpaces(line);

		if (find(line, "Frame Root"))
			processRootMatrix(pFile, object);
		else if (find(line, "Frame Armature"))
			processBoneStructure(pFile, object);
		else if (find(line, "Mesh"))
			processMesh(pFile, object);
		else if (find(line, "SkinWeights"))
			processSkinWeight(pFile, object);
		else if (find(line, "AnimTicksPerSecond"))
			processAnimTicksPerSecond(pFile, object);
		else if (find(line, "Animation"))
			processAnimation(pFile, object);
	}
	fclose(pFile);

	//Calculate Inverse Bind Matrix
	Bone *ptr = object->rootBone;
	do
	{
		if (ptr->currentChild == ptr->numChildren)
		{
			ptr->currentChild = 0;
			if (ptr->father == NULL)
				break;

			Matrix4f c = sW(ptr) *cM(ptr);

			ptr->inverseBindMatrix = c.inverted();
			ptr = ptr->father;
		}
		else
		{
			ptr = ptr->children[ptr->currentChild];
			ptr->father->currentChild++;
		}
	} while (ptr != NULL);

	//NORMALIZE WEIGHTS
	/*for (int i = 0; i < object->numIndices; i++)
	{
	float total = 0;

	Bone *ptr = object->rootBone;
	do
	{
	if (ptr->currentChild == ptr->numChildren)
	{
	ptr->currentChild = 0;
	if (ptr->father == NULL)
	break;

	for (int j = 0; j < ptr->numVertexIndices; j++)
	{
	if (i == ptr->vertexIndices[j])
	{
	total += ptr->weights[j];
	}
	}

	ptr = ptr->father;
	}
	else
	{
	ptr = ptr->children[ptr->currentChild];
	ptr->father->currentChild++;
	}
	} while (ptr != NULL);
	}*/

	return object;
}

void AnimatedBody::processRootMatrix(FILE *stream, AnimatedBody *object)
{
	char nLine[80];
	fgets(nLine, 80, stream);
	Matrix4f frameTransformMatrix;
	for (int i = 0; i < 4; i++)
	{
		fgets(nLine, 80, stream);
		float a, b, c, d;
		sscanf_s(nLine, "%f, %f, %f, %f", &a, &b, &c, &d);
		frameTransformMatrix[i][0] = a;
		frameTransformMatrix[i][1] = b;
		frameTransformMatrix[i][2] = c;
		frameTransformMatrix[i][3] = d;
	}

	object->rootMatrix = frameTransformMatrix.transposed();
}

void AnimatedBody::processBoneStructure(FILE *stream, AnimatedBody *object)
{
	Bone *rootBone = new Bone();
	rootBone->father = NULL;
	rootBone->numChildren = 0;
	rootBone->currentChild = 0;
	Bone *currentBone = rootBone;

	while (true)
	{
		char nLine[80];
		fgets(nLine, 80, stream);
		removeSpaces(nLine);

		if (find(nLine, "} //"))
		{
			if (currentBone->father != NULL)
				currentBone = currentBone->father;
		}
		else if (find(nLine, "//ENDBONES"))
			break;

		char boneName[20];
		if (sscanf_s(nLine, "Frame Armature_%s", boneName, 20))
		{
			fgets(nLine, 80, stream);
			Matrix4f frameTransformMatrix;
			for (int i = 0; i < 4; i++)
			{
				fgets(nLine, 80, stream);
				float a, b, c, d;
				sscanf_s(nLine, "%f, %f, %f, %f", &a, &b, &c, &d);
				frameTransformMatrix[i][0] = a;
				frameTransformMatrix[i][1] = b;
				frameTransformMatrix[i][2] = c;
				frameTransformMatrix[i][3] = d;
			}

			Bone *bone = new Bone();
			for (int i = 0; i < 20; i++)
				bone->name[i] = boneName[i];

			bone->father = currentBone;
			bone->numChildren = 0;
			bone->currentChild = 0;
			bone->frameTransformMatrix = frameTransformMatrix.transposed();

			Bone **tmp = currentBone->children;
			currentBone->children = new Bone*[++currentBone->numChildren];

			for (int i = 0; i < currentBone->numChildren - 1; i++)
				currentBone->children[i] = tmp[i];

			if (currentBone->numChildren - 1 != 0)
				delete[] tmp;

			currentBone->children[currentBone->numChildren - 1] = bone;
			currentBone = currentBone->children[currentBone->numChildren - 1];
		}
	}

	object->setRootBone(rootBone);
}

void AnimatedBody::processMesh(FILE *stream, AnimatedBody *object)
{
	char nLine[80];
	fgets(nLine, 80, stream);

	int numVertices;
	sscanf_s(nLine, "%d", &numVertices);

	Vector3f *vertices = new Vector3f[numVertices];

	for (int i = 0; i < numVertices; i++)
	{
		fgets(nLine, 80, stream);

		float x, y, z;
		sscanf_s(nLine, "%f;%f;%f", &x, &y, &z);
		vertices[i] = Vector3f(x, y, z);
	}
	object->setVertices(numVertices, vertices);

	fgets(nLine, 80, stream);

	int numFaces;
	sscanf_s(nLine, "%d", &numFaces);

	int *indices = new int[numFaces * 4];

	for (int i = 0; i < numFaces; i++)
	{
		fgets(nLine, 80, stream);

		int i0, i1, i2, i3;
		sscanf_s(nLine, "%*d;%d,%d,%d,%d", &i0, &i1, &i2, &i3);
		indices[i * 4] = i0;
		indices[i * 4 + 1] = i1;
		indices[i * 4 + 2] = i2;
		indices[i * 4 + 3] = i3;
	}

	object->setIndices(numFaces * 4, indices);
}

void AnimatedBody::processSkinWeight(FILE *stream, AnimatedBody *object)
{
	char nLine[80];
	fgets(nLine, 80, stream);
	removeSpaces(nLine);

	char boneName[20];
	sscanf_s(nLine, "\"Armature_%s", boneName, 20);
	for (int i = 0; i < 20; i++)
	{
		if (boneName[i] == '\0')
		{
			boneName[i - 1] = '\0';
			boneName[i - 2] = '\0';
		}
	}

	int numIndices;
	fgets(nLine, 80, stream);
	sscanf_s(nLine, "%d", &numIndices);

	int *indices = new int[numIndices];
	for (int i = 0; i < numIndices; i++)
	{
		fgets(nLine, 80, stream);
		int index;
		sscanf_s(nLine, "%d", &index);
		indices[i] = index;
	}

	float *weights = new float[numIndices];
	for (int i = 0; i < numIndices; i++)
	{
		fgets(nLine, 80, stream);
		float weight;
		sscanf_s(nLine, "%f", &weight);
		weights[i] = weight;
	}

	Matrix4f sW;
	for (int i = 0; i < 4; i++)
	{
		fgets(nLine, 80, stream);
		float a, b, c, d;
		sscanf_s(nLine, "%f, %f, %f, %f", &a, &b, &c, &d);
		sW[i][0] = a;
		sW[i][1] = b;
		sW[i][2] = c;
		sW[i][3] = d;
	}

	object->addSkinWeights(boneName, numIndices, indices, weights, sW);
}

void AnimatedBody::processAnimTicksPerSecond(FILE *stream, AnimatedBody *object)
{
	char nLine[80];
	fgets(nLine, 80, stream);

	int animTicksPerSecond;
	sscanf_s(nLine, "%d", &animTicksPerSecond);

	object->setAnimTicksPerSecond(animTicksPerSecond);
}

void AnimatedBody::processAnimation(FILE *stream, AnimatedBody *object)
{
	char nLine[80];
	fgets(nLine, 80, stream);
	removeSpaces(nLine);

	char boneName[20];
	sscanf_s(nLine, "{Armature_%s}", boneName, 20);
	for (int i = 0; i < 20; i++)
	{
		if (boneName[i] == '\0')
		{
			boneName[i - 1] = '\0';
		}
	}

	int numFrames;
	Quaternion *orientationKeys;
	Vector3f *positionKeys;

	while (true)
	{
		fgets(nLine, 80, stream);
		removeSpaces(nLine);

		if (find(nLine, "AnimationKey"))
		{
			fgets(nLine, 80, stream);

			int type;
			sscanf_s(nLine, "%d", &type);

			fgets(nLine, 80, stream);

			sscanf_s(nLine, "%d", &numFrames);

			if (type == 0)
			{
				orientationKeys = new Quaternion[numFrames];

				for (int i = 0; i < numFrames; i++)
				{
					fgets(nLine, 80, stream);

					float x, y, z, w;
					sscanf_s(nLine, "%*d;4;%f, %f, %f, %f", &w, &x, &y, &z);
					orientationKeys[i] = Quaternion(x, y, z, w);
				}
			}
			else if (type == 2)
			{
				positionKeys = new Vector3f[numFrames];

				for (int i = 0; i < numFrames; i++)
				{
					fgets(nLine, 80, stream);

					float x, y, z;
					sscanf_s(nLine, "%*d;3;%f, %f, %f", &x, &y, &z);
					positionKeys[i] = Vector3f(x, y, z);
				}

				break;
			}
		}
	}

	/*0.000  0.000 - 1.000  0.000
	1.000  0.000  0.000  0.000
	0.000 - 1.000  0.000  0.000
	0.000  0.000  0.000  1.000*/

	Matrix4f *frameMatrices = new Matrix4f[numFrames];
	for (int i = 0; i < numFrames; i++)
	{
		Matrix4f T;
		T.initTranslationTransform(positionKeys[i].getX(), positionKeys[i].getY(), positionKeys[i].getZ());

		Matrix4f R;
		R.initRotateTransform(orientationKeys[i].getX(), orientationKeys[i].getY(), orientationKeys[i].getZ(), orientationKeys[i].getW());

		Transform t = Transform(positionKeys[i], orientationKeys[i], Vector3f(1, 1, 1));

		frameMatrices[i] = t.getTransformation();
	}

	object->addBoneAnimation(boneName, numFrames, frameMatrices);
}

bool AnimatedBody::find(char *line, char *c)
{
	for (int i = 0; i < 80; i++)
	{
		if (line[i] != c[i])
			return false;
		if (c[i + 1] == '\0' && (line[i + 1] == ' ' || line[i + 1] == '\n' || line[i + 1] == '\0'))
			return true;
	}

	return false;
}

void AnimatedBody::removeSpaces(char *line)
{
	char l[80];

	for (int i = 0; i < 80; i++)
	{
		if (line[i] != ' ')
		{
			for (int j = 0; j < 80; j++)
			{
				l[j] = line[i + j];
			}
			break;
		}
	}

	for (int i = 0; i < 80; i++)
		line[i] = l[i];
}