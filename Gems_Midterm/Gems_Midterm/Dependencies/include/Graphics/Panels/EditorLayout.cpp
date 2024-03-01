#include "EditorLayout.h"
#include "../ApplicationWindow.h"

EditorLayout& EditorLayout::GetInstance()
{
	static EditorLayout instance;
	return instance;
}

void EditorLayout::InitializeLayout()
{
	windowDockPanel = new WindowDockPanel();
	scenePanel = new SceneHierarchyPanel();
	inspectorPanel = new InspectorPanel();
	toolPanel = new ToolPanel();
	sceneViewportPanel = new SceneViewportPanel();
	gameViewportPanel = new GameViewportPanel();

	scenePanel->viewportCamera = application->viewportCamera;

	sceneViewportPanel->frameBuffer = application->viewportFrameBuffer;
	sceneViewportPanel->viewportCamera = application->viewportCamera;
	sceneViewportPanel->mouseHeld = &application->mouseHeld;

	gameViewportPanel->sceneBuffer = application->gameSceneFrameBuffer;
	//gameViewportPanel->sceneBuffer = application->postProcessing->chromaticAberration.frameBuffer;

	gameViewportPanel->mResolution = ImVec2(application->gameResolutionWidth, application->gameResolutionHeight);
}

void EditorLayout::SetSelectedObjects(std::vector<Object*> objects)
{
	if (selectedObjects.size() != 0)
	{
		for (Object* obj : selectedObjects)
		{
			SetSelectedObjectsState(false);
		}
	}

	selectedObjects.clear();

	selectedObjects = objects;
	SetSelectedObjectsState(true);

}

std::vector<Object*>& EditorLayout::GetSelectedObject()
{
	return selectedObjects;
}

bool EditorLayout::IsViewportHovered()
{
	return hoveredPanelViewport ? sceneViewportPanel->isHovered : gameViewportPanel->isHovered;
}

bool EditorLayout::IsApplicationPlaying()
{
	return application->applicationPlay;
}

bool EditorLayout::IsGameMaximized()
{
	return IsApplicationPlaying() && toolPanel->isMaximizedPressed;
}

void EditorLayout::SetMaximizeState(bool state)
{
	toolPanel->isMaximizedPressed = state;
}

void EditorLayout::SetApplicationPlayState(bool state)
{
	application->applicationPlay = state;
	application->OnPlayStateChanged(state);
}

void EditorLayout::SetHoveredPanel(bool viewport)
{
	hoveredPanelViewport = viewport;
}

void EditorLayout::SetSelectedObjectsState(bool state)
{
	for (Object* obj : selectedObjects)
	{
		obj->isSelected = state;
	}
}

