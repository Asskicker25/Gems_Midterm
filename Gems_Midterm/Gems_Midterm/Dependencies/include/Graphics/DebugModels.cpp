#include "DebugModels.h"
#include "Renderer.h"

DebugModels::DebugModels(const std::string& path)
{
	placeHolder = new Model(path, true);
	placeHolder->isActive = false;

	Initialize();
}

DebugModels::DebugModels(MeshDataHolder& meshData)
{
	this->meshData = meshData;
	placeHolder = new Model();
	placeHolder->LoadModel(meshData,true);
	placeHolder->isActive = false;

	Initialize();
}

void DebugModels::Initialize()
{
	GrowList();
}

void DebugModels::GrowList()
{
	//listOfModels.resize(listOfModels.size() + sizeMultiple, (new Model())->CopyFromModel(*placeHolder));

	listOfModels.resize(listOfModels.size() + sizeMultiple, nullptr);

	for (int i = listOfModels.size() - sizeMultiple; i < listOfModels.size(); i++) 
	{
		listOfModels[i] = new Model();
		listOfModels[i]->CopyFromModel(*placeHolder);

		//renderer->AddModel(listOfModels[i], renderer->solidColorShader);
	}

}

void DebugModels::Clear()
{
	for (Model* model : listOfModels)
	{
		model->isActive = false;
	}

	currentIndex = 0;
}

Model* DebugModels::DrawDebugModel()
{
	Model* model = listOfModels[currentIndex];

	model->isActive = true;

	currentIndex++;

	if (currentIndex >= listOfModels.size())
	{
		GrowList();
	}


	return model;
}
