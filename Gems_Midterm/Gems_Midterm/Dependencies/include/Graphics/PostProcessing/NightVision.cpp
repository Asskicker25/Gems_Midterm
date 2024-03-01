#include "NightVision.h"
#include "../Timer.h"
#include "../Panels/ImguiDrawUtils.h"

void PostProcessing_v1::NightVision::OnPropertyDraw()
{
	if (!ImGui::TreeNodeEx(mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}


	ImGui::TreePop();
}

void PostProcessing_v1::NightVision::SetShaderUniforms()
{
	shader->SetUniform1f("iTime", Timer::GetInstance().elapsedTime);
}
