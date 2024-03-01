#pragma once

#include "Shader.h"

class UnlitColorShader : public Shader
{
public:
	virtual void SetViewPosition(glm::vec3 pos) override;

};

