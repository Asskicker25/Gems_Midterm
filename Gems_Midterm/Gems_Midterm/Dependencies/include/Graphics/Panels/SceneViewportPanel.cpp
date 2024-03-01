#include "SceneViewportPanel.h"
#include "../Renderer.h"
#include "../CameraSystem.h"
#include "../MathUtils.h"
#include "EditorLayout.h"

using namespace MathUtilities;

SceneViewportPanel::SceneViewportPanel()
{
	InputManager::GetInstance().AddListener(this);
}

void SceneViewportPanel::OnRender(float windowSizeX, float windowSizeY)
{
	if (EditorLayout::GetInstance().IsGameMaximized()) return;

	if (!isOpen) return;

	glm::mat4 cameraView = viewportCamera->GetViewMatrix();
	glm::mat4 cameraProjection = viewportCamera->GetMatrix();

	ImGuizmo::SetOrthographic(false);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (!ImGui::Begin("Viewport", &isOpen) || !isEnabled)
	{
		ImGui::End();
		ImGui::PopStyleVar();
		return;
	}

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	isHovered = ImGui::IsWindowHovered();

	if (mViewportSize.x != viewportSize.x || mViewportSize.y != viewportSize.y)
	{
		mViewportSize = viewportSize;
		frameBuffer->Resize(mViewportSize.x, mViewportSize.y);
		viewportCamera->Resize(mViewportSize.x, mViewportSize.y);
	}

	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	ImGui::Image((void*)frameBuffer->GetColorAttachmentId(), mViewportSize, uv0, uv1);
	
	Model* selectedModel = Renderer::GetInstance().selectedModel;

	if (selectedModel != nullptr)
	{
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		glm::mat4 transform = selectedModel->transform.GetTransformMatrix();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
			windowWidth, windowHeight);

		ImGuizmo::Manipulate(
			glm::value_ptr(cameraView),
			glm::value_ptr(cameraProjection),
			guizmoType, ImGuizmo::LOCAL,
			glm::value_ptr(transform));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 pos, rot, scale;
			MathUtils::DecomposeTransform(transform, pos, rot, scale);

			/*glm::quat newQuat = selectedModel->transform.quaternionRotation * glm::quat(transform);
			selectedModel->transform.SetQuatRotation(newQuat);*/

			//Debugger::Print("Rotation : ", rot);

			selectedModel->transform.SetPosition(glm::vec3(pos));
			selectedModel->transform.SetScale(glm::vec3(scale));

			/*glm::vec3 delta = rot - selectedModel->transform.rotation;
			glm::vec3 setRotation = selectedModel->transform.rotation + delta;
			selectedModel->transform.SetRotation(glm::vec3(setRotation));*/
		}
	}

	ImGui::End();

	ImGui::PopStyleVar();
}

void SceneViewportPanel::OnKeyPressed(const int& key)
{
	if (!isHovered) return;
	if (*mouseHeld) return;

	if (key == GLFW_KEY_W)
	{
		guizmoType = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (key == GLFW_KEY_E)
	{
		guizmoType = ImGuizmo::OPERATION::ROTATE;
	}
	else if (key == GLFW_KEY_R)
	{
		guizmoType = ImGuizmo::OPERATION::SCALE;
	}
}

void SceneViewportPanel::OnKeyReleased(const int& key)
{
}

void SceneViewportPanel::OnKeyHeld(const int& key)
{
}
