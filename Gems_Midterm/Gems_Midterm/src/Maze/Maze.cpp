#include <fstream>
#include <Graphics/Renderer.h>
#include <Graphics/UnlitColorMaterial.h>
#include <Graphics/MathUtils.h>
#include <Graphics/Panels/EditorLayout.h>
#include <Graphics/Panels/ImguiDrawUtils.h>

#include "Maze.h"
#include "../Hunter/Hunter.h"

using namespace MathUtilities;

Maze::CellPos Maze::START_CELL_POS = Maze::CellPos(147, 1);
Maze::CellPos Maze::END_CELL_POS = Maze::CellPos(1, 113);

Maze::Maze(unsigned int numOfHunters) : mNumOfHunters{ numOfHunters }
{
	InitializeEntity(this);
	name = "Maze";

	InitializeCriticalSection(&mMaze_CS);

	LoadMazeFromFile("DungeonDetails/Dungeon.txt");
	//PrintMaze();
	SpawnTreasure();
	LoadModels();

}

Maze::~Maze()
{
	DeleteCriticalSection(&mMaze_CS);
}

void Maze::Start()
{
}

void Maze::Update(float deltaTime)
{
}

void Maze::Render()
{
	if (mListOfFloors.size() == 0) return;

	for (unsigned int row = 0; row < ROW_SIZE; row++)
	{
		for (unsigned int column = 0; column < COLUMN_SIZE; column++)
		{
			CellPos cellPos(row, column);

			if (GetCell(cellPos).type == Cell::FLOOR)
			{
				UpdateCellColor(GetCell(cellPos).mCellPos);
			}
		}
	}

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
	mQuad->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(glm::vec4(1));
	mQuad->transform.SetScale(glm::vec3(0.9f));

	for (int i = 0; i < ROW_SIZE; i++)
	{
		mWallInstances[i] = new MeshInstance();
		mWallInstances[i]->CopyFromModel(*mQuad);
		mWallInstances[i]->shader = Renderer::GetInstance().defInstanceShader;
	}

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
			mMazeCells[row][column].mCellPos = CellPos(row, column);
			mMazeCells[row][column].mCellPos.InitializeWeight(mNumOfHunters);

			if (mMazeCells[row][column].type == Cell::WALL)
			{
				/*Model* wall = new Model();
				wall->CopyFromModel(*mQuad);

				wall->transform.SetPosition(cellWorldPosition);

				Renderer::GetInstance().AddModel(wall);
				mListOfWalls.push_back(wall);*/

				mWallInstances[row]->AddTransformValues(glm::vec3(cellWorldPosition), glm::vec3(0), glm::vec3(0.9f));
			}
			else
			{
				if (mMazeCells[row][column].mHasTreasure)
				{
					Model* treasure = new Model();
					treasure->CopyFromModel(*mQuad, false);
					treasure->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(mTreasureColor);
					//treasure->transform.SetScale(glm::vec3(1.0f));
					treasure->transform.SetPosition(cellWorldPosition);

					Renderer::GetInstance().AddModel(treasure);
					mListOfTreasures[GetUniqueId(row, column)] = treasure;

					mNumOfTreasures++;
				}


				/*Model* floor = new Model();
				floor->CopyFromModel(*mQuad, false);
				floor->meshes[0]->material->AsUnlitMaterial()->SetBaseColor(mFloorColor);
				floor->transform.SetPosition(cellWorldPosition);
				floor->transform.position.z -= 0.1f;
				floor->transform.SetScale(glm::vec3(1.0f));

				Renderer::GetInstance().AddModel(floor);
				mListOfFloors[GetUniqueId(row, column)] = floor;*/

				AddAsFloor(CellPos(row, column));
			}
			//if (row == 1 && column == 2) return;
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
	EnterCriticalSection(&mMaze_CS);
	Cell& cell = mMazeCells[row][column];
	LeaveCriticalSection(&mMaze_CS);

	return cell.mCellWorldPosition;
}

glm::vec3 Maze::GetCellPosition(CellPos cellPos)
{
	return GetCellPosition(cellPos.X, cellPos.Y);
}


void Maze::SpawnTreasure()
{
	int randomX;
	int randomY;
	//mNumOfTreasures = TREASURE_COUNT;

	for (unsigned int count = 0; count < TREASURE_COUNT; count++)
	{
		do
		{
			randomX = MathUtils::GetRandomIntNumber(0, ROW_SIZE - 1);
			randomY = MathUtils::GetRandomIntNumber(0, COLUMN_SIZE - 1);

		} while (!IsFloor(randomX, randomY) || HasTreasure(randomX, randomY));

		/*if (count < 13)
		{
		}*/
		mMazeCells[randomX][randomY].mHasTreasure = true;

	}
}


bool Maze::IsFloor(unsigned int row, unsigned int column)
{
	return mMazeCells[row][column].type == Cell::FLOOR;
}

void Maze::AddAsFloor(CellPos cellPos)
{
	AddAdjacentFloor(CellPos(cellPos.X - 1, cellPos.Y), cellPos);
	AddAdjacentFloor(CellPos(cellPos.X + 1, cellPos.Y), cellPos);
	AddAdjacentFloor(CellPos(cellPos.X, cellPos.Y - 1), cellPos);
	AddAdjacentFloor(CellPos(cellPos.X, cellPos.Y + 1), cellPos);
}

void Maze::AddAdjacentFloor(CellPos theMainCell, CellPos theFloorCell)
{
	if (theMainCell.X < 0 || theMainCell.Y < 0 || theMainCell.X > ROW_SIZE - 1 || theMainCell.Y > COLUMN_SIZE - 1) return;
	if (GetCell(theMainCell).type == Cell::WALL) return;

	GetCell(theMainCell).mAdjacentFloors.push_back(&GetCell(theFloorCell).mCellPos);
}

bool Maze::HasTreasure(unsigned int row, unsigned int column)
{
	return mMazeCells[row][column].mHasTreasure;
}

int Maze::GetUniqueId(int row, int column)
{
	return (row << 16) | column;
}

bool Maze::IsTreasureLeftZero()
{
	return mNumOfTreasures == 0;
}

void Maze::OnPropertyDraw()
{
	if (!ImGui::TreeNodeEx("Maze", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawInt("Treasure Count ", mNumOfTreasures);

	ImGui::TreePop();
}

int Maze::GetRandomIntNumber(int minInclusive, int maxInclusive)
{
	EnterCriticalSection(&mMaze_CS);


	//int random = MathUtils::GetRandomIntNumber(minInclusive, maxInclusive);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(minInclusive, maxInclusive);

	int random = distribution(gen);

	LeaveCriticalSection(&mMaze_CS);

	return random;
}

void Maze::RenderWallInstancing()
{
	for (int i = 0; i < ROW_SIZE; i++)
	{
		mWallInstances[i]->DrawShaded(mWallInstances[i]->shader);
	}

}

void Maze::ShutDown()
{
	DeleteCriticalSection(&mMaze_CS);
}

void Maze::UpdateCellColor(CellPos& cellPos)
{
	return;

	std::vector<Object*> listOfSelectedObjects = EditorLayout::GetInstance().GetSelectedObject();

	if (listOfSelectedObjects.size() == 0) return;

	Hunter* hunter = nullptr;
	if (!(hunter = dynamic_cast<Hunter*>(listOfSelectedObjects[0]))) return;


	UnlitColorMaterial* mat = mListOfFloors[GetUniqueId(cellPos.X, cellPos.Y)]->meshes[0]->material->AsUnlitMaterial();

	glm::vec4 color = mat->GetBaseColor();
	color.x = MathUtils::Remap(cellPos.mWeight[hunter->mHunterId], 0, 10, 0, 1);
	mat->SetBaseColor(color);
}

Maze::Cell& Maze::GetCell(CellPos cellPos)
{
	EnterCriticalSection(&mMaze_CS);
	Cell& cell = mMazeCells[cellPos.X][cellPos.Y];
	LeaveCriticalSection(&mMaze_CS);

	return cell;
}

bool Maze::CheckAndCollectTreasure(CellPos& cellPos)
{
	bool collected = false;

	EnterCriticalSection(&mMaze_CS);

	Cell& cell = GetCell(cellPos);

	if (cell.mHasTreasure)
	{
		mListOfTreasures[GetUniqueId(cellPos.X, cellPos.Y)]->isActive = false;
		Debugger::Print("Collected");
		cell.mHasTreasure = false;
		collected = true;
		--mNumOfTreasures;

		if (IsTreasureLeftZero())
		{
			OnAllTreasuresCollected();
		}
	}

	LeaveCriticalSection(&mMaze_CS);

	return collected;
}
