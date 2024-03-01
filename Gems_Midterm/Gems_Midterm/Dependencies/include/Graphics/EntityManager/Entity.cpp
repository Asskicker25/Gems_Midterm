#include "Entity.h"
#include "EntityManager.h"
#include "../Panels/EditorLayout.h"

void Entity::InitializeEntity(Entity* entity)
{
	EntityManager::GetInstance().AddEntity(entity);
}

void Entity::OnSceneDraw()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;
	node_flags |= ImGuiTreeNodeFlags_SpanFullWidth;
	node_flags |= ImGuiTreeNodeFlags_Leaf;

	if (isSelected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		EditorLayout::GetInstance().SetSelectedObjects({ this });
	}
}

void Entity::OnPropertyDraw()
{
	ImGui::InputText("##ObjectName", &name[0], 516);
	ImGui::SameLine();
	ImGui::InputText("##Tag", &tag[0], 516);
	transform.OnPropertyDraw();
}

void Entity::Destroy()
{
	EntityManager::GetInstance().RemoveEntity(entityId);
}

