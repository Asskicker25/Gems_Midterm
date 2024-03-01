#pragma once

#include <Graphics/Scene/BaseScene.h>
#include <Graphics/GameCamera.h>
#include <Graphics/Light.h>

#include "../Maze/Maze.h"
#include "../Hunter/Hunter.h"
#include "../Camera/CameraController.h"

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

	void OnPlayStateChanged(bool state);
	
private:

	CameraController* mCameraController = nullptr;
	MazeApplication* mApplication = nullptr;
	GameCamera* mMainCamera = nullptr;
	Light* mDirLight = nullptr;
	Maze* mMaze = nullptr;

	static const int HUNTERS_COUNT = 1;
	std::vector<Hunter*> mListOfHunters;

};

