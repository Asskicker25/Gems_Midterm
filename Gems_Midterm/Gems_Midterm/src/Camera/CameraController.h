#pragma once
#include <Graphics/GameCamera.h>
#include <Graphics/InputManager/InputManager.h>

class CameraController : public Entity, public iInputListener
{
public:
	CameraController() = default;
	CameraController(GameCamera* gameCamera);

	// Inherited via Entity
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void OnDestroy() override;

	// Inherited via iInputListener
	void OnKeyPressed(const int& key) override;
	void OnKeyReleased(const int& key) override;
	void OnMouseButtonPressed(const int& key) override;
	void OnMouseButtonReleased(const int& key) override;

	virtual void OnPropertyDraw();

private:

	void HandleMove(float deltaTime);
	void HandleRotation(float deltaTime);


	GameCamera* mCamera = nullptr;

	bool mMouseHeld = false;
	bool mKeyQHeld = false;
	bool mKeyEHeld = false;
	
	float mMoveSpeed = 100;
	float columnWidth = 175;

	glm::vec3 mMoveDir = glm::vec3(0);
	glm::vec2 mMouseDelta = glm::vec2(0);
	glm::vec2 mMouseSensitivity  = glm::vec2(10);
	glm::vec2 mLastMousePos = glm::vec2(0);
	glm::vec2 mCurrentMousePos = glm::vec2(0);

	

};

