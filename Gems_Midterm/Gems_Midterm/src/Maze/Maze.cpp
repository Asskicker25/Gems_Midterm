#include "Maze.h"
#include <fstream>

Maze::Maze()
{
	InitializeEntity(this);

	name = "Maze";

	LoadMazeFromFile("DungeonDetails/Dungeon.txt");
	PrintMaze();
}

void Maze::Start()
{
}

void Maze::Update(float deltaTime)
{
}

void Maze::Render()
{
}

void Maze::OnDestroy()
{
}

void Maze::LoadMazeFromFile(const std::string& path)
{
	std::fstream file(path);

	if (!file.is_open())
	{
		Debugger::Print("Couldn't find Maze file.");
	}

	int rowIndex = 0;
	int columnIndex = 0;

	char c;
	while (file.get(c))
	{
		if (c == 'X')
		{
			mMazeCells[rowIndex][columnIndex].type = Cell::WALL;
			columnIndex++;
		}
		else if (c == '.')
		{
			mMazeCells[rowIndex][columnIndex].type = Cell::FLOOR;
			columnIndex++;
		}
		else if (c == '\n')
		{
			rowIndex++;
			columnIndex = 0;
		}
	}
}

void Maze::PrintMaze()
{
	for (int row = 0; row < ROW_SIZE; row++)
	{
		for (int column = 0; column < COLUMN_SIZE; column++)
		{
			if (mMazeCells[row][column].type == Cell::FLOOR)
			{
				printf(".");
			}
			else
			{
				printf("X");
			}
		}

		printf("\n");
	}
}
