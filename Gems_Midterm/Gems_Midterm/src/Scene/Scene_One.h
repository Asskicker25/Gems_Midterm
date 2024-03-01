#pragma once

#include <Graphics/Scene/BaseScene.h>
#include <Graphics/GameCamera.h>
#include <Graphics/Light.h>
#include "../Maze/Maze.h"

using namespace Scene;

class MazeApplication;

class Scene_One : public BaseScene
{
public:

	Scene_One(MazeApplication* application);

	// Inherited via BaseScene
	void Start() override;
	void Update() override;
	void Render() override;
	
private:

	MazeApplication* mApplication = nullptr;
	GameCamera* mMainCamera = nullptr;
	Light* mDirLight = nullptr;
	Maze* mMaze = nullptr;
};
