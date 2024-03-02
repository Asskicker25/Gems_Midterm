#include <Graphics/MathUtils.h>
#include <Graphics/Panels/ImguiDrawUtils.h>

#include "Hunter.h"

using namespace MathUtilities;

Hunter::Hunter(Maze* maze) : mMaze{ maze }
{
	name = "Hunter";
	//LoadModel("Assets/Models/Hunter.fbx");

}

void Hunter::Initialize()
{
	mCurrentCell = mMaze->START_CELL_POS;
	mPrevCell = Maze::CellPos(-1, -1);

	transform.SetPosition(mMaze->GetCellPosition(mMaze->START_CELL_POS));
	transform.position.z += 0.1;
	transform.SetScale(glm::vec3(0.0175f));

	InputManager::GetInstance().AddListener(this);
}


Hunter::~Hunter()
{
}


void Hunter::UpdateHunter(float delatTime, HunterThreadInfo* threadInfo)
{

	mTimeStep += delatTime;

	if (mTimeStep > HUNTER_NEXT_STEP_TIME)
	{
		mTimeStep = 0;
		Move(threadInfo);
	}
}

void Hunter::Update(float deltaTime)
{
	/*Debugger::Print("Run");
	UpdateHunter(deltaTime);*/
}

void Hunter::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_RIGHT)
	{
		HUNTER_NEXT_STEP_TIME -= 0.01;

		if (HUNTER_NEXT_STEP_TIME < 0.01) { HUNTER_NEXT_STEP_TIME = 0.01f; }
	}
	else if (key == GLFW_KEY_LEFT)
	{
		HUNTER_NEXT_STEP_TIME += 0.01;

		if (HUNTER_NEXT_STEP_TIME > 1) { HUNTER_NEXT_STEP_TIME = 1; }
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

void Hunter::MoveToFinalPosition()
{
	transform.SetPosition(mMaze->GetCellPosition(mMaze->END_CELL_POS));
	transform.position.z += 0.1;
}

void Hunter::Move(HunterThreadInfo* threadInfo)
{
	std::vector<Maze::CellPos*>& adjacentCells = mMaze->GetCell(mCurrentCell).mAdjacentFloors;

	EnterCriticalSection(threadInfo->mMaze_CS);

	std::vector<Maze::CellPos*> mListOfLeastWeightCells;
	GetLeastWeightedCells(adjacentCells, mListOfLeastWeightCells);

	if (mListOfLeastWeightCells.size() == 1)
	{
		MoveToPosition(mListOfLeastWeightCells[0], threadInfo);
	}
	else
	{
		int randomIndex = 0;
		do
		{
			randomIndex = mMaze->GetRandomIntNumber(0, mListOfLeastWeightCells.size() - 1);

		} while (*mListOfLeastWeightCells[randomIndex] == mPrevCell);

		MoveToPosition(mListOfLeastWeightCells[randomIndex], threadInfo);
	}

	LeaveCriticalSection(threadInfo->mMaze_CS);
}

void Hunter::MoveToPosition(Maze::CellPos* cellPos, HunterThreadInfo* threadInfo)
{
	mPrevCell = mCurrentCell;

	transform.SetPosition(mMaze->GetCellPosition(*cellPos));
	transform.position.z += 0.1;

	EnterCriticalSection(threadInfo->mMaze_CS);
	mCurrentCell = *cellPos;
	cellPos->mWeight[mHunterId]++;

	if (mMaze->CheckAndCollectTreasure(*cellPos))
	{
		mNumberOfTreasureCollected++;
	}
	LeaveCriticalSection(threadInfo->mMaze_CS);
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

