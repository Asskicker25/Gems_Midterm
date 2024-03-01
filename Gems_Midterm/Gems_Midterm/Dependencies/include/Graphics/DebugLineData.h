#pragma once

#include "Debugger.h"
#include "Vertex.h"
#include "MeshDataHolder.h"

class DebugLineData : public MeshDataHolder
{
public:

	DebugLineData();

	void SetStartPoint(glm::vec3 startPoint);
	void SetEndPoint(glm::vec3 endPoint);
};



