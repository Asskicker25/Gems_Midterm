#pragma once

#include "BaseMaterial.h"

#include "CubeMapTexture.h"

class SkyBoxMaterial : public BaseMaterial
{
public:
	SkyBoxMaterial();

	CubeMapTexture* skyBoxTexture;

	// Inherited via BaseMaterial
	void UpdateMaterial(Shader* shader) override;
	void ResetMaterial(Shader* shader) override;
	BaseMaterial* CloneMaterial() override;
};

