#pragma once

#include "Debugger.h"
#include "Transform.h"
#include "Texture/RenderTexture.h"
#include "Model.h"
#include "PostProcessing/PostProcessing.h"

enum ECameraType
{
	PERSPECTIVE = 0,
	ORTHOGRAPHIC = 1
};

class Camera : public Model
{

protected:

	ECameraType cameraType;
	float cameraWidth = 1280;
	float cameraHeight = 720;
	float nearPlane;
	float farPlane;
	float fov;

	float aspectRatio;

	glm::mat4 cameraMatrix;

	const char* items[2] = { "Perspective", "Orthographic"};

	int item_current = 0;

	void UpdateProjectionMatrix();

public:

	bool isViewPortCamera = false;
	bool applyPostProcessing = false;
	RenderTexture* renderTexture = nullptr;

	PostProcessing_v1::PostProcessing* postProcessing;

	Camera();
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov = 0);
	void InitializeCamera();
	void ChangeCameraType(ECameraType type);

	bool IsOrthographic();

	void Resize(float width, float height);

	void SetCameraForward();

	void SetCameraWidth(const float& width);
	void SetCameraHeight(const float& height);


	Transform* GetTransform();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetMatrix() const { return cameraMatrix; }

	// Inherited via Entity
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

	void OnPropertyDraw() override;
	void OnSceneDraw() override;


};

