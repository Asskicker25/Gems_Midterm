#include "Emission.h"
#include "../../Panels/ImguiDrawUtils.h"


void System_Particle::Emission::AddBurst(Burst burstInfo)
{
	mListOfBursts.push_back(burstInfo);
}

void System_Particle::Emission::RemoveBurst(int index)
{
	std::vector<Burst>::iterator it = mListOfBursts.begin() + index;

	mListOfBursts.erase(it);
}

void System_Particle::Emission::DrawBurst(Burst& burstInfo, int index)
{
	if (!ImGui::TreeNodeEx(("Burst " + std::to_string(index)).c_str()))
	{
		return;
	}
	ImGuiUtils::DrawFloat("Time", burstInfo.time);
	ImGuiUtils::DrawInt("Count", burstInfo.count);
	ImGuiUtils::DrawInt("Cycles", burstInfo.cycles);
	ImGuiUtils::DrawFloat("Interval", burstInfo.interval);
	ImGuiUtils::DrawFloat("Probability", burstInfo.probability);

	ImGui::TreePop();
}

void System_Particle::Emission::OnSceneDraw()
{
}

void System_Particle::Emission::OnPropertyDraw()
{
	ImGuiUtils::DrawFloat("Rate Over Time ", rateOverTime);


	

	for (int i = 0; i < mListOfBursts.size(); i++)
	{
		DrawBurst(mListOfBursts[i], i);
	}

}


