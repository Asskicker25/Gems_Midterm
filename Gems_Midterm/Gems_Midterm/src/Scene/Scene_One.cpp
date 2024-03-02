#include "Scene_One.h"
#include "../AppSettings.h"
#include "../MazeApplication.h"
#include "../Thread.h"
#include <chrono>

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

	mCameraController = new CameraController(mMainCamera);

	mMaze = new Maze(HUNTERS_COUNT);
	mMaze->OnAllTreasuresCollected = [this]()
		{
			OnAllTreasureCollected();
		};

	Model hunterModel("Assets/Models/Hunter.fbx", true);

	threadInfos = new HunterThreadInfo[HUNTERS_COUNT];

	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		Hunter* hunter = new Hunter(mMaze);
		hunter->CopyFromModel(hunterModel);
		hunter->mHunterId = i;
		hunter->Initialize();
		Renderer::GetInstance().AddModel(hunter);
		mListOfHunters.push_back(hunter);

		threadInfos[i].mDesiredUpdateTime = FIXED_STEP_TIME;
		threadInfos[i].mHunter = hunter;
		threadInfos[i].isAlive = true;
		threadInfos[i].sleepTime = 1;
		threadInfos[i].mMaze_CS = &mMaze->mMaze_CS;
		threadInfos[i].ThreadId = i;


		threads[i] = chBEGINTHREADEX(
			NULL, 
			0, 
			UpdateHunterThread,
			(PVOID)&threadInfos[i],
			0, 
			&threadInfos[i].ThreadId);
	}

}

void Scene_One::Update()
{

}

void Scene_One::Render()
{
	mMaze->RenderWallInstancing();
}

void Scene_One::ShutDown()
{
	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		threadInfos[i].isAlive = false;
	}

	WaitForMultipleObjects(HUNTERS_COUNT, threads, TRUE, INFINITE);

	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		CloseHandle(threads[i]);
	}

	mMaze->ShutDown();

}

void Scene_One::OnPlayStateChanged(bool state)
{
	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		threadInfos[i].mIsRunning = state;
	}
}

void Scene_One::OnAllTreasureCollected()
{

	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		threadInfos[i].isAlive = false;
	}

	//WaitForMultipleObjects(HUNTERS_COUNT, threads, TRUE, INFINITE);
	Sleep(1);

	std::system("cls");

	printf("All Treasures Collected!!!!\n");

	int mTotal = 0;

	for (int i = 0; i < HUNTERS_COUNT; i++)
	{
		mListOfHunters[i]->MoveToFinalPosition();
		printf("Treasure Collected by Hunter %u : %u\n", i, mListOfHunters[i]->mNumberOfTreasureCollected);
		mTotal += mListOfHunters[i]->mNumberOfTreasureCollected;
	}

	printf("Total Treasure Collected : %u\n", mTotal);

}


DWORD UpdateHunterThread(LPVOID lpParameter)
{
	HunterThreadInfo* threadInfo = (HunterThreadInfo*)lpParameter;

	double currentTime = glfwGetTime();
	double lastTime = currentTime;
	double deltaTime = 0.0f;

	double timeStep = 0.0f;

	while (threadInfo->isAlive)
	{
		//std::cout << std::endl;

		if (threadInfo->mIsRunning)
		{
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			if (deltaTime > threadInfo->mDesiredUpdateTime) { deltaTime = threadInfo->mDesiredUpdateTime; }

			timeStep += deltaTime;

			threadInfo->mHunter->UpdateHunter(deltaTime, threadInfo);

			/*if (timeStep >= threadInfo->mDesiredUpdateTime)
			{
				timeStep = 0;

				threadInfo->mHunter->UpdateHunter(deltaTime, threadInfo);
			}*/

		}
		Sleep(1);

	}
	return 0;
}
