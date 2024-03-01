#include "BaseScene.h"
#include "../EntityManager/EntityManager.h"
#include "../Renderer.h"
#include "../CameraSystem.h"
#include "../LightManager.h"
#include "../InputManager/InputManager.h"

void Scene::BaseScene::Cleanup()
{
	EntityManager::GetInstance().Clear();
	Renderer::GetInstance().ClearModelList();
	CameraSystem::GetInstance().ClearCameras();
	LightManager::GetInstance().Clear();
	InputManager::GetInstance().Clear();
}
