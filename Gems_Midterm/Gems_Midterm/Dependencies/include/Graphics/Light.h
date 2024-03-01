#pragma once

#include "Material.h"
#include "Model.h"

enum LightType 
{
	Point = 0,
	Spot = 1,
	Directional = 2,
};
 
class Light : public Model
{

private:
	Material material;

public:
	bool isActive = true;

	LightType lightType;

	glm::vec4 attenuation;

	float intensity;
	float innerAngle;
	float outerAngle;


	Light(LightType lightType);
	Light();

	void InitializeLight(LightType lightType);

	glm::vec4 GetLightColor();
	glm::vec4 GetAmbientColor();
	float GetAmbientValue();

	void SetLightColor(glm::vec4 color);
	void SetAmbientColor(glm::vec4 color);
	void SetAmbientValue(float ambientValue);

	void OnDestroy() override;
	void OnPropertyDraw() override;
	void OnSceneDraw() override;
};

