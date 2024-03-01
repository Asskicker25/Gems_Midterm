#include "ToolPanel.h"
#include "../Renderer.h"
#include "EditorLayout.h"

void ToolPanel::OnRender(float windowSizeX, float windowSizeY)
{
	if (!isOpen) return;

	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_MenuBar;

	float panelSizeX = windowSizeX;
	float panelSizeY = windowSizeY / 40;

	float panelPosX = 0;
	float panelPosY = 0 + panelSizeY / 10;

	ImGui::SetNextWindowSize(ImVec2(panelSizeX, panelSizeY));
	ImGui::SetNextWindowPos(ImVec2(panelPosX, panelPosY));

	if (!ImGui::Begin("ToolPanel", &isOpen, window_flags) || !isEnabled)
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar())
	{
	
		if (!Renderer::GetInstance().showNormals)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			if (ImGui::Button("View Normals"))
			{
				Renderer::GetInstance().showNormals = true;
			}
			ImGui::PopStyleVar();
		}
		else
		{
			if (ImGui::Button("View Normals"))
			{
				Renderer::GetInstance().showNormals = false;
			}
		}

		if (!isMaximizedPressed)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			if (ImGui::Button("Maximize"))
			{
				isMaximizedPressed = true;
			}
			ImGui::PopStyleVar();
		}
		else
		{
			if (ImGui::Button("Maximize"))
			{
				isMaximizedPressed = false;
			}
		}
		

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(panelSizeX * 0.5f,0));
		ImGui::Spacing();
		ImGui::PopStyleVar();

		bool isPlaying = EditorLayout::GetInstance().IsApplicationPlaying();

		if (ImGui::Button(isPlaying ? "Pause" : "Play"))
		{
			EditorLayout::GetInstance().SetApplicationPlayState(!isPlaying);
		}

		ImGui::EndMenuBar();
	}


	ImGui::End();
}
