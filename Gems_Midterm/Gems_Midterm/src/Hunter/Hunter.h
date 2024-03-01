#pragma once
#include <Graphics/Model.h>
#include <Graphics/InputManager/InputManager.h>

#include "HunterThreadInfo.h"
#include "../Maze/Maze.h"

extern DWORD WINAPI UpdateHunterThread(LPVOID lpParameter);

class Hunter : public Model, public iInputListener
{
public:
	~Hunter();
	Hunter() = default;
	Hunter(Maze* maze);

	void InitializeThread();
	void UpdateHunter(float delatTime);

	virtual void OnKeyPressed(const int& key);

private:
	void Move();
	void MoveToPosition(Maze::CellPos* cellPos);
	
	void GetLeastWeightedCells(std::vector<Maze::CellPos>& adjacentFloor, std::vector<Maze::CellPos*>& filteredList);

	Maze* mMaze = nullptr;
	HunterThreadInfo mThreadInfo;

	float mTimeStep = 0;
	float HUNTER_NEXT_STEP_TIME = 0.1f;

	Maze::CellPos mCurrentCell;
	Maze::CellPos mPrevCell;

	static constexpr float FIXED_STEP_TIME = 1.0f / 60.f;
};

