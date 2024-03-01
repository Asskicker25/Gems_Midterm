#pragma once
#include <unordered_map>

#include "BaseScene.h"
#include <string>

namespace Scene
{
	class SceneManager
	{
	public:

		static SceneManager& GetInstance();

		void Update();
		void Render();

		void AddScene(const std::string& sceneName, BaseScene* scene);
		void RemoveScene(const std::string& sceneName);
		void ChangeScene(const std::string& name);

		BaseScene* GetScene(const std::string& name);
		BaseScene* GetCurrentScene();

	private:

		std::string mCurrentScene = "";
		std::unordered_map<std::string, BaseScene*> mListOfScene;

	};
}


