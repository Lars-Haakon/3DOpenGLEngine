#ifndef TESTGAME_H
#define TESTGAME_H
#include "Game.h"
#include "Camera.h"
#include "Scene.h"
#include "AnimatedBody.h"

class TestGame : Game
{
public:
	TestGame();
	~TestGame();

	virtual void input();
	virtual void update();
	virtual void render();

	Camera *getCamera();
private:
	Camera *cam;
	Scene *scene;
	AnimatedBody *body;
};

#endif