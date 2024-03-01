#pragma once

#include "Shaders/Shader.h"
#include "Object.h"

class Material;
class UnlitColorMaterial;
class SkyBoxMaterial;


class BaseMaterial : public Object
{
public:
	virtual void UpdateMaterial(Shader* shader) = 0;
	virtual void ResetMaterial(Shader* shader) = 0;
	virtual BaseMaterial* CloneMaterial() = 0;

	Material* AsMaterial();
	UnlitColorMaterial* AsUnlitMaterial();
	SkyBoxMaterial* AsSkyBoxMaterial();

	virtual void OnPropertyDraw();
	virtual void OnSceneDraw();

};

