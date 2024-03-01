#pragma once

#include "Shader.h"
#include "../Debugger.h"

class ShaderSystem
{
public:

	static ShaderSystem& GetInstance();

	void AddShader(Shader* shader);
	void RemoveShader(Shader* shader);

	std::vector<Shader*> mListOfShaders;
};

