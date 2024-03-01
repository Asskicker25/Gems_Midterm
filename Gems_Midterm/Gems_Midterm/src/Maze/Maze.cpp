#include <fstream>
#include <Graphics/Renderer.h>
#include <Graphics/UnlitColorMaterial.h>
#include <Graphics/MathUtils.h>

#include "Maze.h"

using namespace MathUtilities;

Maze::CellPos Maze::START_CELL_POS = Maze::CellPos(147, 1);
Maze::CellPos Maze::END_CELL_POS = Maze::CellPos(1, 113);

Maze::Maze()
{
	InitializeEntity(this);

	name = "Maze";

	LoadMazeFromFile("DungeonDetails/Dungeon.txt");
	//PrintMaze();
	SpawnTreasure();
	LoadModels();
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

	unsigned int rowIndex = 0;
	unsigned int columnIndex = 0;

	char c;
	while (file.get(c))
	{
		if (c == 'X')
		{
			mMazeCells[rowIndex][columnIndex].type = Cell::WALL;
			columnIndex++;
			mWallCount++;
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
	for (unsigned int row = 0; row < ROW_SIZE; row++)
	{
		for (unsigned int column = 0; column < COLUMN_SIZE; column++)
		{
			mMazeCells[row][column].mCellPos = Maze::CellPos(row, column);

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

void Maze::LoadModels()
{
	mQuad = new Model("res/Models/DefaultQuad.fbx", true);
	mQuad->name = "Quad";
	mQuad->shader = Renderer::GetInstance().solidColorShader;
	mQuad->meshes[0]->material = new UnlitColorMaterial();

	/*Model* meh = new Model();
	meh->CopyFromModel(*mQuad);*/

	mListOfWalls.reserve(mWallCount);
	mListOfTreasures.reserve(TREASURE_COUNT);

	for (unsigned int row = 0; row < ROW_SIZE; row++)
	{
		for (unsigned int column = 0; column < COLUMN_SIZE; column++)
		{
			glm::vec3 cellWorldPosition = glm::vec3(
				column * CELL_SIZE,
				row * -CELL_SIZE,
				0);

			mMazeCells[row][column].mCellWorldPosition = cellWorldPosition;

			if (mMazeCells[row][column].type == Cell::WALL)
			{
				Model* floor = new Model();
				floor->CopyFromModel(*mQuad);

				floor->transform.SetPosition(cellWorldPosition);

				Renderer::GetInstance().AddModel(floor);
				mListOfWalls.push_back(floor);
			}
			else if (mMazeCells[row][column].mHasTreasure)
			{
				Model* treasure = new Model();
				treasure->CopyFromModel(*mQuad, false);
				treasure->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(mTreasureColor);
				treasure->transform.SetScale(glm::vec3(1.0f));
				treasure->transform.SetPosition(cellWorldPosition);

				Renderer::GetInstance().AddModel(treasure);
				mListOfTreasures.push_back(treasure);
			}

		}

		//if (row == 2) return;
	}


	Model* startPos = new Model();
	startPos->CopyFromModel(*mQuad, true);
	startPos->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(mStartPosColor);
	startPos->transform.SetPosition(GetCellPosition(START_CELL_POS));


	Model* endPos = new Model();
	endPos->CopyFromModel(*mQuad, true);
	endPos->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(mEndPosColor);
	endPos->transform.SetPosition(GetCellPosition(END_CELL_POS));


}

glm::vec3 Maze::GetCellPosition(unsigned int row, unsigned int column)
{
	return mMazeCells[row][column].mCellWorldPosition;
}

void Maze::SpawnTreasure()
{
	int randomX;
	int randomY;

	for (unsigned int count = 0; count < TREASURE_COUNT; count++)
	{
		do
		{
			randomX = MathUtils::GetRandomIntNumber(0, ROW_SIZE - 1);
			randomY = MathUtils::GetRandomIntNumber(0, COLUMN_SIZE - 1);

		} while (!IsFloor(randomX, randomY) && HasTreasure(randomX, randomY));

		mMazeCells[randomX][randomY].mHasTreasure = true;

	}
}

glm::vec3 Maze::GetCellPosition(CellPos cellPos)
{
	return GetCellPosition(cellPos.X, cellPos.Y);
}

bool Maze::IsFloor(unsigned int row, unsigned int column)
{
	return mMazeCells[row][column].type == Cell::FLOOR;
}

bool Maze::HasTreasure(unsigned int row, unsigned int column)
{
	return mMazeCells[row][column].mHasTreasure;
}
