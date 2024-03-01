#include <Graphics/MathUtils.h>
#include <Graphics/Panels/ImguiDrawUtils.h>

#include "Hunter.h"

using namespace MathUtilities;

Hunter::Hunter(Maze* maze) : mMaze{ maze }
{
	name = "Hunter";
	//LoadModel("Assets/Models/Hunter.fbx");

}

void Hunter::InitializeThread()
{
	mThreadInfo.mDesiredUpdateTime = FIXED_STEP_TIME;
	mThreadInfo.mHunter = this;
	mThreadInfo.isAlive = true;
	mThreadInfo.sleepTime = 1;
	mThreadInfo.mMaze_CS = &mMaze->mMaze_CS;

	mThreadInfo.threadHandle = CreateThread(
		NULL,							// lpThreadAttributes,
		0,								// dwStackSize,
		UpdateHunterThread,				// lpStartAddress,
		(void*)&mThreadInfo,			//  lpParameter,
		0,								// dwCreationFlags (0 or CREATE_SUSPENDED)
		&mThreadInfo.ThreadId);			// lpThreadId

}

void Hunter::Initialize()
{
	mCurrentCell = mMaze->START_CELL_POS;
	mPrevCell = Maze::CellPos(-1, -1);

	transform.SetPosition(mMaze->GetCellPosition(mMaze->START_CELL_POS));
	transform.position.z += 0.1;
	transform.SetScale(glm::vec3(0.0175f));

	InputManager::GetInstance().AddListener(this);
	InitializeThread();
}

Hunter::~Hunter()
{
	mThreadInfo.isAlive = false;
	WaitForSingleObject(mThreadInfo.threadHandle, INFINITE);
	CloseHandle(mThreadInfo.threadHandle);
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
		if (threadInfo->mIsRunning)
		{
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			if (deltaTime > threadInfo->mDesiredUpdateTime) { deltaTime = threadInfo->mDesiredUpdateTime; }

			timeStep += deltaTime;

			if (timeStep >= threadInfo->mDesiredUpdateTime)
			{
				timeStep = 0;

				threadInfo->mHunter->UpdateHunter(deltaTime);
			}

			Sleep(threadInfo->sleepTime);
		}

	}


	return 0;
}

void Hunter::UpdateHunter(float delatTime)
{
	mTimeStep += delatTime;

	if (mTimeStep > HUNTER_NEXT_STEP_TIME)
	{
		mTimeStep = 0;
		Move();
	}
}

void Hunter::Update(float deltaTime)
{
	/*Debugger::Print("Run");
	UpdateHunter(deltaTime);*/
}

void Hunter::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_SPACE)
	{
		mThreadInfo.mIsRunning = true;
	}
}

void Hunter::OnPropertyDraw()
{
	Model::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("Hunter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawInt("TreasureCount ", mNumberOfTreasureCollected);


	ImGui::TreePop();
}

void Hunter::Move()
{
	std::vector<Maze::CellPos*>& adjacentCells = mMaze->GetCell(mCurrentCell).mAdjacentFloors;

	std::vector<Maze::CellPos*> mListOfLeastWeightCells;
	GetLeastWeightedCells(adjacentCells, mListOfLeastWeightCells);

	if (mListOfLeastWeightCells.size() == 1)
	{
		MoveToPosition(mListOfLeastWeightCells[0]);
	}
	else
	{
		int randomIndex = 0;
		do
		{
			randomIndex = mMaze->GetRandomIntNumber(0, mListOfLeastWeightCells.size() - 1);

		} while (*mListOfLeastWeightCells[randomIndex] == mPrevCell);

		MoveToPosition(mListOfLeastWeightCells[randomIndex]);
	}
}

void Hunter::MoveToPosition(Maze::CellPos* cellPos)
{
	mPrevCell = mCurrentCell;

	transform.SetPosition(mMaze->GetCellPosition(*cellPos));
	transform.position.z += 0.1;

	mCurrentCell = *cellPos;
	cellPos->mWeight[mHunterId]++;

	if (mMaze->CheckAndCollectTreasure(*cellPos))
	{
		mNumberOfTreasureCollected++;
	}

	//mMaze->UpdateCellColor(cellPos);

}

void Hunter::GetLeastWeightedCells(std::vector<Maze::CellPos*>& adjacentFloor, std::vector<Maze::CellPos*>& filteredList)
{
	int minWeight = adjacentFloor[0]->mWeight[mHunterId];

	for (Maze::CellPos* cellPos : adjacentFloor)
	{
		if (cellPos->mWeight[mHunterId] < minWeight)
		{
			minWeight = cellPos->mWeight[mHunterId];
			filteredList.clear();
		}

		if (cellPos->mWeight[mHunterId] == minWeight)
		{
			filteredList.push_back(cellPos);
		}
	}
}

