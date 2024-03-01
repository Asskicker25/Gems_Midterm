#pragma once

#include "Debugger.h"
#include "Model.h"
#include "MeshDataHolder.h"

class DebugModels
{
private:
	int sizeMultiple = 512;

	int currentIndex = 0;

	std::vector<Model*> listOfModels;

	Model* placeHolder;
	MeshDataHolder meshData;

public:
	DebugModels(const std::string& path);
	DebugModels(MeshDataHolder& meshData);

	void Initialize();
	void GrowList();
	void Clear();

	Model* DrawDebugModel();

};

