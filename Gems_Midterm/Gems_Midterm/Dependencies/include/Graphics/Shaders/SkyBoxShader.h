#pragma once

#include "Shader.h"

class SkyBoxShader : public Shader
{
public:
	virtual void SetView(glm::mat4 matrix) override;
	virtual void SetViewPosition(glm::vec3 pos) override;
};

