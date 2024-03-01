#pragma once

#include "BasePanel.h"

class ToolPanel : public BasePanel
{
public:
	bool isMaximizedPressed = true;

	void OnRender(float windowSizeX, float windowSizeY) override;
};

