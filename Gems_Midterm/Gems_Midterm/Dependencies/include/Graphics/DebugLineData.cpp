#include "DebugLineData.h"

DebugLineData::DebugLineData()
{
	vertices = {
		 {glm::vec3(-1.0f, -1.0f, -1.0f)},  // 0
		{glm::vec3(1.0f, -1.0f, -1.0f)},  // 1
		{glm::vec3(1.0f, -1.0f, -1.0f)},  // 2
	};

	indices = { 0,1,1};
}

void DebugLineData::SetStartPoint(glm::vec3 startPoint)
{
	vertices[0].positions = startPoint;
}

void DebugLineData::SetEndPoint(glm::vec3 endPoint)
{
	vertices[1].positions = endPoint;
}
