#include "UnlitColorMaterial.h"
#include "Texture/Texture.h"

UnlitColorMaterial::UnlitColorMaterial()
{
	baseColor = glm::vec4(1);
	alphaMask = new Texture("res/Textures/DefaultTextures/Opacity_Default.png");
	
}


glm::vec4 UnlitColorMaterial::GetBaseColor()
{
	return baseColor;
}

void UnlitColorMaterial::SetBaseColor(glm::vec4 color)
{
	this->baseColor = color;
}

void UnlitColorMaterial::UpdateMaterial(Shader* shader)
{
	shader->Bind();
	shader->SetUniform4f("solidColor", this->GetBaseColor().x, this->GetBaseColor().y, this->GetBaseColor().z,1);
	

	if (this->alphaMask != nullptr)
	{
		this->alphaMask->SetTextureSlot(2);
		shader->SetUniform1i("texture_opacity", 2);
		this->alphaMask->Bind();
	}

	shader->SetUniform1f("alphaCutoffThreshold", alphaCutoffThreshold);

}

void UnlitColorMaterial::ResetMaterial(Shader* shader)
{
}

BaseMaterial* UnlitColorMaterial::CloneMaterial()
{
	UnlitColorMaterial* material = new UnlitColorMaterial();

	material->SetBaseColor(this->GetBaseColor());
	material->alphaCutoffThreshold = this->alphaCutoffThreshold;
	material->alphaMask = this->alphaMask;

	return material;
}

void UnlitColorMaterial::OnPropertyDraw()
{
	ImGui::Text("Unlit Color Material");
	BaseMaterial::OnPropertyDraw();
}

void UnlitColorMaterial::OnSceneDraw()
{
}
