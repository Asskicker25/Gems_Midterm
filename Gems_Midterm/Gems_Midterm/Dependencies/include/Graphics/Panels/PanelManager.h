#pragma once

#include <vector>
#include "BasePanel.h"

class PanelManager
{
public:

	PanelManager();
	static PanelManager& GetInstance();

	void AddPanel(BasePanel* panel);
	void DeletePanel(BasePanel* panel);
	void EnablePanel(BasePanel* panel);
	void DisablePanel(BasePanel* panel);

	void Update(float windowSizeX, float windowSizeY);

private:

	std::vector<BasePanel*> listOfPanels;
};

