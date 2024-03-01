#include "HeatDistortion.h"
#include "../Timer.h"
#include "../Panels/ImguiDrawUtils.h"

void PostProcessing_v1::HeatDistortion::OnPropertyDraw()
{
	if (!ImGui::TreeNodeEx(mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawFloat( "Amount", mAmount);

	ImGui::TreePop();
}

void PostProcessing_v1::HeatDistortion::SetShaderUniforms()
{
	shader->SetUniform1f("iTime", Timer::GetInstance().elapsedTime);
	shader->SetUniform1f("amount", mAmount);
}
