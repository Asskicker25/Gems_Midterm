#include "BaseMaterial.h"
#include "Material.h"
#include "UnlitColorMaterial.h"
#include "SkyBoxMaterial.h"

Material* BaseMaterial::AsMaterial()
{
	return (Material*)this;
}

UnlitColorMaterial* BaseMaterial::AsUnlitMaterial()
{
	return (UnlitColorMaterial*)this;
}

SkyBoxMaterial* BaseMaterial::AsSkyBoxMaterial()
{
	return (SkyBoxMaterial*)this;
}

void BaseMaterial::OnPropertyDraw()
{
}

void BaseMaterial::OnSceneDraw()
{
}
