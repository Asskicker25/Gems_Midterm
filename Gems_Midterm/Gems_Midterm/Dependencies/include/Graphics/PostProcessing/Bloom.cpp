#include "Bloom.h"
#include "../Renderer.h"
#include "../Panels/ImguiDrawUtils.h"


using namespace PostProcessing_v1;

void Bloom::OnPropertyDraw()
{

	if (!ImGui::TreeNodeEx("Bloom", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}


	ImGuiUtils::DrawFloat("Amount", mAmount);
	/*ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 150);
	ImGui::Text("Amount");
	ImGui::NextColumn();

	ImGui::InputFloat("###BloomAmount", &mAmount,0,0,"%.2f");

	ImGui::Columns(1);*/

	ImGui::TreePop();
}

void Bloom::OnSceneDraw()
{
}

void Bloom::InitializeFrameBuffer(int width, int height)
{
	brightnessExtractShader = new Shader("res/Shader/PostProcessing/BrightnessExtractShader.shader", false, Shader::ALPHA_OPAQUE, false);
	blurShader = new Shader("res/Shader/PostProcessing/BlurShader.shader",false, Shader::ALPHA_OPAQUE, false);


	FrameBufferSpecification brightnessSpec;
	brightnessSpec.width = 800;
	brightnessSpec.height = 600;
	brightnessSpec.attachments = { FrameBufferTextureFormat::RGBA8 , FrameBufferTextureFormat::DEPTH24STENCIL8 };
	brightnessFrameBuffer = new FrameBuffer(brightnessSpec);

	FrameBufferSpecification blurSpec;
	blurSpec.width = 800;
	blurSpec.height = 600;
	blurSpec.attachments = { FrameBufferTextureFormat::RGBA8,  FrameBufferTextureFormat::RGBA8, };

	blurFrameBuffer = new FrameBuffer(blurSpec);
	
	
}

void Bloom::OnApplyEffect(FrameBuffer* sceneBuffer)
{

	ExtractBrightnessBuffer(sceneBuffer);
	BlurBuffer(sceneBuffer);
	CombineSceneAndBlur(sceneBuffer);
}

unsigned int Bloom::GetColorAttachmentId()
{
	return blurFrameBuffer->GetColorAttachmentId(1);
}

void Bloom::ExtractBrightnessBuffer(FrameBuffer* sceneBuffer)
{

	brightnessFrameBuffer->Bind();

	Renderer::GetInstance().Clear();

	brightnessExtractShader->Bind();

	GLCALL(glActiveTexture(GL_TEXTURE0));

	brightnessExtractShader->SetUniform1i("sceneTexture", 0);

	GLCALL(glBindTexture(GL_TEXTURE_2D, sceneBuffer->GetColorAttachmentId()));

	RenderQuad::Render();

	brightnessFrameBuffer->UnBind();
}

void Bloom::BlurBuffer(FrameBuffer* buffer)
{
	bool horizontal = true;
	bool firstIteration = true;

	blurShader->Bind();

	blurFrameBuffer->Bind();
	Renderer::GetInstance().Clear();

	for (int i = 0; i < mAmount; i++)
	{
		blurFrameBuffer->Bind();


		blurShader->SetUniform1i("horizontal", horizontal);

		GLCALL(glBindTexture(GL_TEXTURE_2D, firstIteration ? brightnessFrameBuffer->GetColorAttachmentId()
			: blurFrameBuffer->GetColorAttachmentId(horizontal ? 0 : 1)));

		RenderQuad::Render();

		horizontal = !horizontal;

		if (firstIteration)
		{
			firstIteration = false;
		}
	}

	blurFrameBuffer->UnBind();

}

void Bloom::CombineSceneAndBlur(FrameBuffer* sceneBuffer)
{
	sceneBuffer->Bind();

	combineShader->Bind();

	GLCALL(glActiveTexture(GL_TEXTURE0));
	combineShader->SetUniform1i("sceneTexture", 0);
	GLCALL(glBindTexture(GL_TEXTURE_2D, sceneBuffer->GetColorAttachmentId()));


	GLCALL(glActiveTexture(GL_TEXTURE1));
	combineShader->SetUniform1i("effectTexture", 1);
	combineShader->SetUniform1i("isEffectEnabled", isEnabled);
	GLCALL(glBindTexture(GL_TEXTURE_2D, GetColorAttachmentId()));

	RenderQuad::Render();

	sceneBuffer->UnBind();
}

