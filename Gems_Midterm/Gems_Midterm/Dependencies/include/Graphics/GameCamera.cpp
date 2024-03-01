#include "GameCamera.h"
#include "CameraSystem.h"

GameCamera::GameCamera()
{
	CameraSystem::GetInstance().AddCamera(this);
}

GameCamera::~GameCamera()
{
	CameraSystem::GetInstance().RemoveCamera(this);
}

void GameCamera::SetRenderTexture(RenderTexture* texture)
{
	renderTexture = texture;

	FrameBufferSpecification spec;
	spec.width = cameraWidth;
	spec.height = cameraHeight;
	spec.attachments = { FrameBufferTextureFormat::RGBA8 , FrameBufferTextureFormat::DEPTH24STENCIL8 };


	renderTexture->InitializeRenderTexture(spec);
}
