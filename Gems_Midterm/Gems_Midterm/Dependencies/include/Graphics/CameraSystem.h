#pragma once

#include "Camera.h"

class CameraSystem
{
public:

	static CameraSystem& GetInstance();

	void AddCamera(Camera* camera);
	void RemoveCamera(Camera* camera);
	void ClearCameras();

	Camera* GetMainCamera();

	std::vector<Camera*> mListOfCameras;

	Camera* viewportCamera = nullptr;

};

