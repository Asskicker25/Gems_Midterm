#pragma once
#include "Material.h"

class MeshInstanceMaterial : public Material
{
	// Inherited via BaseMaterial
	void UpdateMaterial(Shader* shader) override;
	void ResetMaterial(Shader* shader) override;
	BaseMaterial* CloneMaterial() override;
};

