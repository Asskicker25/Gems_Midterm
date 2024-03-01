#include "PanelManager.h"

PanelManager::PanelManager()
{

}

PanelManager& PanelManager::GetInstance()
{
	static PanelManager instance;
	return instance;
}

void PanelManager::AddPanel(BasePanel* panel)
{
	listOfPanels.push_back(panel);
}

void PanelManager::DeletePanel(BasePanel* panel)
{
	listOfPanels.erase(std::remove(listOfPanels.begin(), listOfPanels.end(), panel), listOfPanels.end());
}

void PanelManager::EnablePanel(BasePanel* panel)
{
	panel->isEnabled = true;
}

void PanelManager::DisablePanel(BasePanel* panel)
{
	panel->isEnabled = false;
}

void PanelManager::Update(float windowSizeX, float windowSizeY)
{
	for (BasePanel* panel : listOfPanels)
	{
		if (!panel->isEnabled) continue;

		panel->OnRender(windowSizeX, windowSizeY);
	}
}

