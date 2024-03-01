
#pragma once

#include "Camera.h"

class GameCamera : public Camera
{
public:

	GameCamera();
	~GameCamera();

	void SetRenderTexture(RenderTexture* texture);
};

