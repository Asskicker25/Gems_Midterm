#pragma once

#include "Debugger.h"
#include "Light.h"
#include "Shaders/Shader.h"

class LightManager
{
private:
	std::vector<Light*> lights;
	std::vector<Shader*> shaders;
	Shader* lightShader = nullptr;

	static const unsigned int NUMBER_OF_LIGHTS_IN_USE = 15;

public:

	LightManager();
	~LightManager();

	static LightManager& GetInstance();

	void SetLightShader(Shader& lightShader);
	void AddLight(Light* light);
	void AddShader(Shader* shader);
	void RemoveLight(Light* light);

	void AddLight(Light& light);
	void AddShader(Shader& shader);
	void RemoveLight(Light& light);
	std::vector<Light*> GetListOfLights();

	void RenderLight();

	void Clear();
};

