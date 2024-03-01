
#pragma once

#include "Debugger.h"
#include "Texture/BaseTexture.h"
#include "BaseMaterial.h"


enum AmbientType
{
	Value,
	Color
};

class Material : public BaseMaterial
{

private:
	glm::vec4 baseColor;
	glm::vec4 ambientColor;

public:

	BaseTexture* diffuseTexture;
	BaseTexture* specularTexture;
	BaseTexture* alphaMask;

	AmbientType ambientType;

	float ambientValue;
	float specularValue;
	float shininess = 32;
	float alphaCutoffThreshold = 0.5f;

	bool useMaskTexture = false;

	glm::vec2 textureTiling = glm::vec2(1.0f);

	Material();
	Material(const Material& material);
	~Material();

	void ResetMaterial();
	float GetSpecularValue();
	glm::vec4 GetBaseColor();
	glm::vec4 GetAmbientColor();

	void SetBaseColor(glm::vec4 color);
	void SetAmbientColor(glm::vec4 color);

	// Inherited via BaseMaterial
	void UpdateMaterial(Shader* shader) override;
	void ResetMaterial(Shader* shader) override;

	// Inherited via BaseMaterial
	BaseMaterial* CloneMaterial() override;

	void OnPropertyDraw() override;
	void OnSceneDraw() override;
};


