#pragma once

#include <Graphics/EntityManager/EntityManager.h>
#include <Graphics/Model.h>

class Maze : public Entity
{
public:
	struct Cell
	{
		enum Type
		{
			WALL = 0,
			FLOOR = 1,
		};

		Type type = Type::WALL;
		bool mHasTreasure = false;
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

	Model* mQuad = nullptr;

	static const int ROW_SIZE = 149;
	static const int COLUMN_SIZE = 115;
	static constexpr float CELL_SIZE = 2.0f;

	Cell mMazeCells[ROW_SIZE][COLUMN_SIZE];

	std::vector<Model*> mListOfModels;

};

