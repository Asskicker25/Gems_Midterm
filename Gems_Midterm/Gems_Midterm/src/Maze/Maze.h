#pragma once

#include <Graphics/EntityManager/EntityManager.h>
#include <Graphics/Model.h>

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

		unsigned int X = 0;
		unsigned int Y = 0;
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

		std::vector<CellPos> mAdjacentFloors;

	};

	Maze();

	// Inherited via Entity
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void OnDestroy() override;

private:
	void LoadMazeFromFile(const std::string& path);
	void PrintMaze();
	void LoadModels();
	void SpawnTreasure();

	void AddAsFloor(CellPos cellPos);
	void AddAdjacentFloor(CellPos theMainCell, CellPos theFloorCell);

	Cell& GetCell(CellPos cellPos);

	glm::vec3 GetCellPosition(unsigned int row, unsigned int column);
	glm::vec3 GetCellPosition(CellPos cellPos);

	bool IsFloor(unsigned int row, unsigned int column);
	bool HasTreasure(unsigned int row, unsigned int column);

	Model* mQuad = nullptr;

	int mWallCount = 0;

	static CellPos START_CELL_POS;
	static CellPos END_CELL_POS;

	static const int ROW_SIZE = 149;
	static const int COLUMN_SIZE = 115;
	static const int TREASURE_COUNT = 250;
	static constexpr float CELL_SIZE = 2.0f;


	glm::vec4 mStartPosColor = glm::vec4(0.1f, 0.8f, 0.1f, 1.0f);
	glm::vec4 mEndPosColor = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
	glm::vec4 mTreasureColor = glm::vec4(0.9f, 0.9f, 0.1f, 1.0f);


	Cell mMazeCells[ROW_SIZE][COLUMN_SIZE];

	std::vector<Model*> mListOfWalls;
	std::unordered_map<std::string, Model*> mListOfTreasures;

};

