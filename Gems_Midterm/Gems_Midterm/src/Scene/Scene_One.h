#pragma once

#include <Graphics/Scene/BaseScene.h>
#include <Graphics/GameCamera.h>
#include <Graphics/Light.h>

#include "../Maze/Maze.h"
#include "../Hunter/Hunter.h"
#include "../Camera/CameraController.h"

#include <Windows.h>

using namespace Scene;

class MazeApplication;

extern DWORD WINAPI UpdateHunterThread(LPVOID lpParameter);

class Scene_One : public BaseScene
{
public:

	Scene_One(MazeApplication* application);

	// Inherited via BaseScene
	void Start() override;
	void Update() override;
	void Render() override;

	void ShutDown();

	void OnPlayStateChanged(bool state);
	void OnAllTreasureCollected();
	
private:

	CameraController* mCameraController = nullptr;
	MazeApplication* mApplication = nullptr;
	GameCamera* mMainCamera = nullptr;
	Light* mDirLight = nullptr;
	Maze* mMaze = nullptr;

	static const int HUNTERS_COUNT = 64;
	static constexpr float FIXED_STEP_TIME = 1.0f / 60.f;

	HANDLE threads[HUNTERS_COUNT];
	HunterThreadInfo* threadInfos = nullptr;

	std::vector<Hunter*> mListOfHunters;

};

