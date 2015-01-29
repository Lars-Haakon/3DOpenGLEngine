#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include "Rigidbody.h"
#include "Geometry.h"
#include "BSPMap.h"

class Scene
{
public:
	Scene(char *sceneName);
	~Scene();

	void update();
	void render();

	Face *getSceneFaces();
	int getNumSceneFaces();
private:
	LinkedList<Mesh> meshes; // meshes for the scene
	LinkedList<Material> materials; // 1 material for each mesh
	Shader *ambientShader;
	Shader *directionalShader;
	Shader *pointShader;
	Shader *spotShader;
};

#endif