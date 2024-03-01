#pragma once

#include "iInputListener.h"
#include <vector>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class InputManager
{
private:
	std::vector<iInputListener*> inputListeners;

	class Pimpl;
	Pimpl* pimpl;

	float mMouseX = 0;
	float mMouseY = 0;
	float mAxisX = 0;
	float mAxisY = 0;

	glm::vec2 mMouseDelta = glm::vec2(0);

	bool mLeftHeld = false;
	bool mRightHeld = false;
	bool mUpHeld = false;
	bool mDownHeld = false;

	void CalculateAxis();

public:
	InputManager();
	~InputManager();

	static InputManager& GetInstance();

	void AddListener(iInputListener* inputListener);
	void RemoveListener(iInputListener* inputListener);

	void OnKeyPressed(const int& key);
	void OnKeyReleased(const int& key);
	void OnKeyHeld();

	void OnMouseButtonPressed(const int& key);
	void OnMouseButtonReleased(const int& key);


	float GetMouseX();
	float GetMouseY();
	float GetAxisX();
	float GetAxisY();
	glm::vec2 GetMouseDelta();

	void SetMousePos(float x, float y);
	void SetMouseDelta(glm::vec2 delta);

	void Update();
	void Clear();
};

