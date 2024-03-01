#pragma once
#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>

namespace ImGuiUtils
{
	extern bool DrawVector3ImGui(std::string label, glm::vec3& values, float resetValue, float& columnWidth);
	extern bool DrawVector2ImGui(std::string label, glm::vec2& values, float resetValue, float columnWidth);
	extern bool DrawDropDown(std::string label, int& currentItem, const char* const* items, int size);
	extern bool DrawFloat(std::string label, float& value);
	extern bool DrawDragFloat(std::string label, float& value, float speed,float min, float max);
	extern bool DrawInt(std::string label, int& value);
	extern bool DrawBool(std::string label, bool& value);
}
