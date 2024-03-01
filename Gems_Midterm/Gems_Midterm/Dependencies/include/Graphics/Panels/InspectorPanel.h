#pragma once

#include "BasePanel.h"

class InspectorPanel : public BasePanel
{
public:

	// Inherited via BasePanel
	void OnRender(float windowSizeX, float windowSizeY) override;

private:

	void DrawObjectProperties();
};

