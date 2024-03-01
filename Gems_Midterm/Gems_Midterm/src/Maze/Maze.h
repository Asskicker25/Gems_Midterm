#pragma once

#include <Graphics/EntityManager/EntityManager.h>
#include <Graphics/Model.h>
#include <Graphics/MeshInstance.h>
#include <Windows.h>

class Maze : public Entity
{
public:

	struct CellPos
	{
		CellPos() = default;
		CellPos(unsigned int X, unsigned int Y)
		{
			this->X = X;
			this->Y = Y;
		}

		void InitializeWeight(unsigned int num)
		{
			for (int i = 0; i < num; i++)
			{
				mWeight[i] = 0;
			}
		}

		bool operator==(const CellPos& other)
		{
			if (other.X != X) return false;
			if (other.Y != Y) return false;

			return true;
		}

		unsigned int X = 0;
		unsigned int Y = 0;

		std::unordered_map<int, int> mWeight;
		//int mWeight = 0;
	};

	struct Cell
	{
		enum Type
		{
			WALL = 0,
			FLOOR = 1,
		};

		bool mHasTreasure = false;
		Type type = Type::WALL;

		CellPos mCellPos;
		glm::vec3 mCellWorldPosition = glm::vec3(0);

		std::vector<CellPos*> mAdjacentFloors;

	};

	Maze(unsigned int numOfHunters);
	~Maze();

	// Inherited via Entity
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void OnDestroy() override;
	virtual void OnPropertyDraw();

	void RenderWallInstancing();

	int GetRandomIntNumber(int minInclusive, int maxInclusive);

	static CellPos START_CELL_POS;
	static CellPos END_CELL_POS;

	glm::vec3 GetCellPosition(unsigned int row, unsigned int column);
	glm::vec3 GetCellPosition(CellPos cellPos);

	void UpdateCellColor(CellPos& cellPos);
	bool CheckAndCollectTreasure(CellPos& cellPos);

	Cell& GetCell(CellPos cellPos);

	CRITICAL_SECTION mMaze_CS;

private:
	void LoadMazeFromFile(const std::string& path);
	void PrintMaze();
	void LoadModels();
	void SpawnTreasure();
	int GetUniqueId(int row, int column);

	void AddAsFloor(CellPos cellPos);
	void AddAdjacentFloor(CellPos theMainCell, CellPos theFloorCell);

	
	bool IsFloor(unsigned int row, unsigned int column);
	bool HasTreasure(unsigned int row, unsigned int column);

	static const int ROW_SIZE = 149;
	static const int COLUMN_SIZE = 115;
	static const int TREASURE_COUNT = 250;
	static constexpr float CELL_SIZE = 2.0f;

	MeshInstance* mWallInstances[ROW_SIZE];
	Model* mQuad = nullptr;

	int mWallCount = 0;
	int mNumOfTreasures = 0;
	unsigned int mNumOfHunters = 0;

	glm::vec4 mStartPosColor = glm::vec4(0.1f, 0.8f, 0.1f, 1.0f);
	glm::vec4 mEndPosColor = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
	glm::vec4 mTreasureColor = glm::vec4(0.9f, 0.9f, 0.1f, 1.0f);
	glm::vec4 mFloorColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	Cell mMazeCells[ROW_SIZE][COLUMN_SIZE];

	std::vector<Model*> mListOfWalls;
	std::unordered_map<int, Model*> mListOfTreasures;
	std::unordered_map<int, Model*> mListOfFloors;

};

