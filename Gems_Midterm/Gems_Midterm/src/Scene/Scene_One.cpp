#include "Scene_One.h"
#include "../AppSettings.h"

Scene_One::Scene_One(MazeApplication* application)
{
	this->mApplication = application;
}

void Scene_One::Start()
{
	mMainCamera = new GameCamera();
	mMainCamera->InitializeCamera(PERSPECTIVE, GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT, 0.1f, 500.0f, 65.0f);
	mMainCamera->transform.SetPosition(glm::vec3(127.4f, -147.6f, 242.4f));
	mMainCamera->transform.SetRotation(glm::vec3(0, 0, 0));
	//mMainCamera->applyPostProcessing = true;

	mDirLight = new Light();
	mDirLight->transform.SetScale(glm::vec3(0.1f));
	mDirLight->transform.SetPosition(glm::vec3(0, 0, 3));
	mDirLight->transform.SetRotation(glm::vec3(10, 40, 0));
	mDirLight->InitializeLight(Directional);
	mDirLight->intensity = 0.8;


	mMaze = new Maze();
}

void Scene_One::Update()
{
}

void Scene_One::Render()
{
}
