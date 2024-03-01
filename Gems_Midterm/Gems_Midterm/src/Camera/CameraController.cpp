#include "CameraController.h"
#include <Graphics/Panels/ImguiDrawUtils.h>
#include <Graphics/Timer.h>

CameraController::CameraController(GameCamera* gameCamera) : mCamera{ gameCamera }
{
	name = "CameraController";
	InitializeEntity(this);
	InputManager::GetInstance().AddListener(this);
}

void CameraController::Start()
{
}

void CameraController::Update(float deltaTime)
{
	//if (!mMouseHeld) return;

	HandleMove(deltaTime);
	//HandleRotation(deltaTime);
}

void CameraController::Render()
{
}

void CameraController::OnDestroy()
{
}

void CameraController::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_Q)
	{
		mKeyQHeld = true;
	}
	else if (key == GLFW_KEY_E)
	{
		mKeyEHeld = true;
	}
}

void CameraController::OnKeyReleased(const int& key)
{
	if (key == GLFW_KEY_Q)
	{
		mKeyQHeld = false;
	}
	else if (key == GLFW_KEY_E)
	{
		mKeyEHeld = false;
	}
}

void CameraController::OnMouseButtonPressed(const int& key)
{
	if (key == GLFW_MOUSE_BUTTON_2)
	{
		mMouseHeld = true;
		mCurrentMousePos = glm::vec2(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
		mLastMousePos = mCurrentMousePos;
	}
}

void CameraController::OnMouseButtonReleased(const int& key)
{
	if (key == GLFW_MOUSE_BUTTON_2)
	{
		mMouseHeld = false;
	}
}

void CameraController::OnPropertyDraw()
{
	Entity::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("CameraController", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawVector3ImGui("Move Dir", mMoveDir,0, columnWidth);
	ImGuiUtils::DrawVector2ImGui("Mouse Delta", mMouseDelta,0, columnWidth);
	ImGuiUtils::DrawVector2ImGui("Mouse Sens", mMouseSensitivity,0, columnWidth);

	ImGui::TreePop();
}

void CameraController::HandleMove(float deltaTime)
{
	mMoveDir.x = InputManager::GetInstance().GetAxisX();
	mMoveDir.z = InputManager::GetInstance().GetAxisY();

	mMoveDir.y = 0;

	mMoveDir.y -= mKeyQHeld ? 1 : 0;
	mMoveDir.y += mKeyEHeld ? 1 : 0;

	mCamera->transform.position += mCamera->transform.GetRight() * mMoveDir.x * deltaTime * mMoveSpeed;
	mCamera->transform.position += mCamera->transform.GetUp() * mMoveDir.y * deltaTime * mMoveSpeed;
	mCamera->transform.position += mCamera->transform.GetForward() * mMoveDir.z * deltaTime * mMoveSpeed;

}

void CameraController::HandleRotation(float deltaTime)
{
	mCurrentMousePos = glm::vec2(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());

	if (mLastMousePos == glm::vec2(0)) { mLastMousePos = mCurrentMousePos; return; }

	mMouseDelta = mLastMousePos - mCurrentMousePos;

	glm::vec3 newCamRotation = mCamera->transform.rotation;

	newCamRotation.x += mMouseDelta.y * mMouseSensitivity.y * Timer::GetInstance().deltaTime;
	newCamRotation.y += mMouseDelta.x * mMouseSensitivity.x * Timer::GetInstance().deltaTime;

	mCamera->transform.SetRotation(newCamRotation);

	mLastMousePos = mCurrentMousePos;
}


