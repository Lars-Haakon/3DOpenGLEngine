#include "TestGame.h"

TestGame::TestGame()
{
	cam = new Camera(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f));
	Transform::setCamera(cam);

	//scene = new Scene("scenes/test.bsp");
	body = AnimatedBody::loadAO("models/simple4.x");
}

TestGame::~TestGame()
{
	delete cam;
	delete scene;
	delete body;
}

void TestGame::input()
{
	cam->input();
}

/*
UPDATE PIPELINE
1. update with generators
2. integrate
3. generate collision contacts
4. contact resolution
5. repeat from step 1
*/

void TestGame::update()
{
	// 1 APPLY GENERATORS
	// 2 INTEGRATE
	//scene->update();
	// 3 GENERATE COLLISION CONTACTS
	//ContactData *data = CollisionDetection::generateCollisionContacts(scene);
	// 4 RESOlVE CONTACTS
	//ContactResolver::resolveContacts(data);
	// 5 REPEAT
}

void TestGame::render()
{
	//scene->render();
	body->render();
}

Camera *TestGame::getCamera()
{
	return cam;
}