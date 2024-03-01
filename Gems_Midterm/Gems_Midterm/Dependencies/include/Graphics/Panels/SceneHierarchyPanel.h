#pragma once

#include "BasePanel.h"

class EntityManager;
class LightManager;
class Camera;

class SceneHierarchyPanel : public BasePanel
{

public:

	SceneHierarchyPanel();

	// Inherited via BasePanel
	void OnRender(float windowSizeX, float windowSizeY) override;

	void ShowEntities();
	void ShowLights();
	void ShowCamera();

	Camera* viewportCamera = nullptr;

private:

	EntityManager* entityManager = nullptr;
	LightManager* lightManager = nullptr;
};

