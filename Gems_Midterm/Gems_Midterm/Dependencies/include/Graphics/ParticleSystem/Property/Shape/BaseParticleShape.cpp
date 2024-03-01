#include "BaseParticleShape.h"
#include "../../../Panels/ImguiDrawUtils.h"

void System_Particle::BaseParticleShape::OnPropertyDraw()
{
	ImGuiUtils::DrawVector3ImGui("Position", position, 0, columnWidth);
	ImGuiUtils::DrawVector3ImGui("Rotation", rotation, 0, columnWidth);
	ImGuiUtils::DrawVector3ImGui("Scale", scale, 0, columnWidth);

	ImGuiUtils::DrawBool("RandomDir", randomizeDirection);
}

