#pragma once

#include "WindowDockPanel.h"
#include "SceneHierarchyPanel.h"
#include "InspectorPanel.h"
#include "ToolPanel.h"
#include "SceneViewportPanel.h"
#include "GameViewportPanel.h"

class Object;
class ApplicationWindow;

class EditorLayout
{

public:

	static EditorLayout& GetInstance();

	void InitializeLayout();
	void SetSelectedObjects(std::vector<Object*> objects);
	std::vector<Object*>& GetSelectedObject();

	bool IsViewportHovered();
	bool IsApplicationPlaying();
	bool IsGameMaximized();

	void SetMaximizeState(bool state);
	void SetApplicationPlayState(bool state);

	void SetHoveredPanel(bool viewport = true);

	ApplicationWindow* application;

private:
	bool hoveredPanelViewport = true;

	WindowDockPanel* windowDockPanel;
	SceneHierarchyPanel* scenePanel;
	InspectorPanel* inspectorPanel;
	ToolPanel* toolPanel;
	SceneViewportPanel* sceneViewportPanel;
	GameViewportPanel* gameViewportPanel;

	std::vector<Object*> selectedObjects;

	void SetSelectedObjectsState(bool state);

};

