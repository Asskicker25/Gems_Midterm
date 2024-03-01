#include "ChromaticAberration.h"
#include "../Timer.h"
#include "../Panels/ImguiDrawUtils.h"

using namespace PostProcessing_v1;

void ChromaticAberration::SetShaderUniforms()
{
	shader->SetUniform1f("iTime", Timer::GetInstance().elapsedTime);
	shader->SetUniform1f("displaceAmount", mAmount);
}

void ChromaticAberration::OnPropertyDraw()
{

	if (!ImGui::TreeNodeEx(mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawFloat("Amount", mAmount);

	ImGui::TreePop();

}

