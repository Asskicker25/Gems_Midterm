#pragma once

#include <imgui/imgui.h>
#include "../Object.h"

class BasePanel
{
public:

	bool isOpen = true;
	bool isEnabled = true;
	bool isHovered = false;
	bool* isPlaying = nullptr;
	
	BasePanel();
	virtual void OnRender(float windowSizeX, float windowSizeY) = 0;

private: 
	const char* panelName;
	
};

