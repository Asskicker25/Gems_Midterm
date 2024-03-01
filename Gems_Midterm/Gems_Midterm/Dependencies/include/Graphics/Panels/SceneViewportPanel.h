#pragma once

#include "BasePanel.h"
#include "../FrameBuffer.h"
#include "../Camera.h"
#include "../InputManager/InputManager.h"
#include <imgui/ImGuizmo.h>


class SceneViewportPanel : public BasePanel, public iInputListener
{
public:
	SceneViewportPanel();
	// Inherited via BasePanel
	void OnRender(float windowSizeX, float windowSizeY) override;

	FrameBuffer* frameBuffer = nullptr;
	Camera* viewportCamera = nullptr;
	ImVec2 mViewportSize;

	glm::mat4 identityMat = glm::mat4(1);

	ImGuizmo::OPERATION guizmoType = ImGuizmo::OPERATION::TRANSLATE;

	bool* mouseHeld = nullptr;
private:


	// Inherited via iInputListener
	void OnKeyPressed(const int& key) override;

	void OnKeyReleased(const int& key) override;

	void OnKeyHeld(const int& key) override;

};

