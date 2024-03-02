#pragma once
#include <Graphics/Model.h>
#include <Graphics/InputManager/InputManager.h>

#include "HunterThreadInfo.h"
#include "../Maze/Maze.h"


class Hunter : public Model, public iInputListener
{
public:
	~Hunter();
	Hunter() = default;
	Hunter(Maze* maze);

	unsigned int mHunterId = 0;

	void Initialize();
	void UpdateHunter(float delatTime, HunterThreadInfo* threadInfo);
	virtual void Update(float deltaTime) override;

	virtual void OnKeyPressed(const int& key);
	virtual void OnPropertyDraw();

	void MoveToFinalPosition();

	int mNumberOfTreasureCollected = 0;

private:
	void Move(HunterThreadInfo* threadInfo);
	void MoveToPosition(Maze::CellPos* cellPos, HunterThreadInfo* threadInfo);
	
	void GetLeastWeightedCells(std::vector<Maze::CellPos*>& adjacentFloor, std::vector<Maze::CellPos*>& filteredList);

	Maze* mMaze = nullptr;


	float mTimeStep = 0;
	float HUNTER_NEXT_STEP_TIME = 0.1f;

	Maze::CellPos mCurrentCell;
	Maze::CellPos mPrevCell;

	static constexpr float FIXED_STEP_TIME = 1.0f / 60.f;
};

