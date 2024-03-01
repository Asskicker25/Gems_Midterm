#include "SceneManager.h"

using namespace Scene;

SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}

void Scene::SceneManager::Update()
{
	if (mCurrentScene == "") return;

	GetCurrentScene()->Update();
}

void Scene::SceneManager::Render()
{
	if (mCurrentScene == "") return;

	GetCurrentScene()->Render();
}

void Scene::SceneManager::AddScene(const std::string& sceneName, BaseScene* scene)
{
	mListOfScene[sceneName] = scene;
	scene->mName = sceneName;
}

void Scene::SceneManager::RemoveScene(const std::string& sceneName)
{
	mListOfScene.erase(sceneName);
}

void Scene::SceneManager::ChangeScene(const std::string& name)
{
	if (mCurrentScene != "")
	{
		GetCurrentScene()->Cleanup();
	}
	
	mCurrentScene = name;

	GetCurrentScene()->Start();
}

BaseScene* Scene::SceneManager::GetScene(const std::string& name)
{
	return mListOfScene[name];
}

BaseScene* Scene::SceneManager::GetCurrentScene()
{
	return mListOfScene[mCurrentScene];
}
