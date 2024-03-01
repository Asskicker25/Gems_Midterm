#include "SinglePassEffect.h"
#include "../Renderer.h"

using namespace PostProcessing_v1;


void SinglePassEffect::InitializeEffect(const std::string& name, const std::string& shaderPath)
{
	shader = new Shader(shaderPath, false, Shader::ALPHA_OPAQUE, false);

	FrameBufferSpecification specs;
	specs.width = 1920;
	specs.height = 1080;
	specs.attachments = { FrameBufferTextureFormat::RGBA8 };

	frameBuffer = new FrameBuffer(specs);

	this->mName = name;
}

void SinglePassEffect::OnApplyEffect(FrameBuffer* sceneBuffer)
{

#pragma region DoEffect


	frameBuffer->Bind();

	Renderer::GetInstance().Clear();

	shader->Bind();

	SetShaderUniforms();

	GLCALL(glActiveTexture(GL_TEXTURE0));
	shader->SetUniform1i("sceneTexture", 0);
	GLCALL(glBindTexture(GL_TEXTURE_2D, sceneBuffer->GetColorAttachmentId()));


	RenderQuad::Render();

	frameBuffer->UnBind();


#pragma endregion

#pragma region ApplyEffectToScene

	sceneBuffer->Bind();

	Renderer::GetInstance().Clear();

	combineShader->Bind();

	GLCALL(glActiveTexture(GL_TEXTURE0));
	combineShader->SetUniform1i("sceneTexture", 0);
	GLCALL(glBindTexture(GL_TEXTURE_2D, frameBuffer->GetColorAttachmentId()));


	GLCALL(glActiveTexture(GL_TEXTURE1));
	combineShader->SetUniform1i("effectTexture", 1);
	combineShader->SetUniform1i("isEffectEnabled", false);
	GLCALL(glBindTexture(GL_TEXTURE_2D, sceneBuffer->GetColorAttachmentId()));

	RenderQuad::Render();

	sceneBuffer->UnBind();
#pragma endregion

}
