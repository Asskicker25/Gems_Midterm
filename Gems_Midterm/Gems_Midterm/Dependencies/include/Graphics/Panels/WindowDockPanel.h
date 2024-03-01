#pragma once

#include "BasePanel.h"

class WindowDockPanel : public BasePanel
{
	// Inherited via BasePanel
	void OnRender(float windowSizeX, float windowSizeY) override;
};

