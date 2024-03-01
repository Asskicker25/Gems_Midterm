#include "ShaderSystem.h"

ShaderSystem& ShaderSystem::GetInstance()
{
	static ShaderSystem instance;
	return instance;
}

void ShaderSystem::AddShader(Shader* shader)
{
	for (Shader* it : mListOfShaders)
	{
		if (it == shader) return;
	}

	mListOfShaders.push_back(shader);
}

void ShaderSystem::RemoveShader(Shader* shader)
{
	mListOfShaders.erase(std::remove(mListOfShaders.begin(), mListOfShaders.end(), shader), mListOfShaders.end());
}
