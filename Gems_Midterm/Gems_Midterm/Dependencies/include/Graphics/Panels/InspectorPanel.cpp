#include "InspectorPanel.h"
#include "EditorLayout.h"

void InspectorPanel::OnRender(float windowSizeX, float windowSizeY)
{
	if (EditorLayout::GetInstance().IsGameMaximized()) return;

	if (!isOpen) return;

	if (!ImGui::Begin("Inspector", &isOpen))
	{
		ImGui::End();
		return;
	}

	DrawObjectProperties();

	ImGui::End();
}

void InspectorPanel::DrawObjectProperties()
{
	std::vector<Object*> selectedObjects = EditorLayout::GetInstance().GetSelectedObject();

	if (selectedObjects.size() == 0) return;

	for (Object* obj : selectedObjects)
	{
		obj->OnPropertyDraw();
	}

}
