#include "SkyBoxMaterial.h"

SkyBoxMaterial::SkyBoxMaterial()
{
    skyBoxTexture = new CubeMapTexture();
}

void SkyBoxMaterial::UpdateMaterial(Shader* shader)
{

    this->skyBoxTexture->SetTextureSlot(0);
    shader->SetUniform1i("skybox", 0);
    this->skyBoxTexture->Bind();

}

void SkyBoxMaterial::ResetMaterial(Shader* shader)
{
}

BaseMaterial* SkyBoxMaterial::CloneMaterial()
{
    return nullptr;
}
