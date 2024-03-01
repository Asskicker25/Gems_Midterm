#pragma once

#include <Graphics/EntityManager/EntityManager.h>


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

	static const int ROW_SIZE = 149;
	static const int COLUMN_SIZE = 115;

	Cell mMazeCells[ROW_SIZE][COLUMN_SIZE];

};

