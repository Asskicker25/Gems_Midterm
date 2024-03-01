#pragma once
#include "Model.h"
#include "Transform.h"

class MeshInstance : public Model
{
public:

	std::vector<glm::mat4> mListOfModelMatrixes;

	void AddTransformValues(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	virtual void DrawShaded(Shader* shader) override;

};

