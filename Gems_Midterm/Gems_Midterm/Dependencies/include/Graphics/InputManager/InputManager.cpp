#include <unordered_set>
#include "InputManager.h"
#include "../Debugger.h"

class InputManager::Pimpl
{
public:
	std::unordered_set<int> keysHeld;

	void AddKeyHeld(const int& key);
	void RemoveKeyHeld(const int& key);
};

void InputManager::Pimpl::AddKeyHeld(const int& key)
{
	keysHeld.insert(key);
}

void InputManager::Pimpl::RemoveKeyHeld(const int& key)
{
	keysHeld.erase(key);
}


InputManager::InputManager() : pimpl { new Pimpl() }
{
}

InputManager::~InputManager()
{
	delete pimpl;
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::AddListener(iInputListener* inputListener)
{
	inputListeners.push_back(inputListener);
}

void InputManager::RemoveListener(iInputListener* inputListener)
{
	inputListeners.erase(std::remove(inputListeners.begin(), inputListeners.end(), inputListener), inputListeners.end());
}

void InputManager::OnKeyPressed(const int& key)
{
	pimpl->AddKeyHeld(key);

	for (iInputListener* listener : inputListeners)
	{
		listener->OnKeyPressed(key);
	}

	if (key == GLFW_KEY_A)
	{
		mLeftHeld = true;
	}
	else if (key == GLFW_KEY_D)
	{
		mRightHeld = true;
	}
	else if (key == GLFW_KEY_W)
	{
		mUpHeld = true;
	}
	else if (key == GLFW_KEY_S)
	{
		mDownHeld = true;
	}
}

void InputManager::OnKeyReleased(const int& key)
{
	pimpl->RemoveKeyHeld(key);

	for (iInputListener* listener : inputListeners)
	{
		listener->OnKeyReleased(key);
	}

	if (key == GLFW_KEY_A)
	{
		mLeftHeld = false;
	}
	else if (key == GLFW_KEY_D)
	{
		mRightHeld = false;
	}
	else if (key == GLFW_KEY_W)
	{
		mUpHeld = false;
	}
	else if (key == GLFW_KEY_S)
	{
		mDownHeld = false;
	}
}

void InputManager::OnKeyHeld()
{
	for (iInputListener* listener : inputListeners)
	{
		for (auto& key : pimpl->keysHeld)
		{
			listener->OnKeyHeld(key);
		}
	}
}

void InputManager::OnMouseButtonPressed(const int& key)
{
	for (iInputListener* listener : inputListeners)
	{
		listener->OnMouseButtonPressed(key);
	}
}

void InputManager::OnMouseButtonReleased(const int& key)
{
	for (iInputListener* listener : inputListeners)
	{
		listener->OnMouseButtonReleased(key);
	}
}

float InputManager::GetMouseX()
{
	return mMouseX;
}

float InputManager::GetMouseY()
{
	return mMouseY;
}

float InputManager::GetAxisX()
{
	return mAxisX;
}

float InputManager::GetAxisY()
{
	return mAxisY;
}

glm::vec2 InputManager::GetMouseDelta()
{
	return mMouseDelta;
}

void InputManager::SetMousePos(float x, float y)
{
	mMouseX = x;
	mMouseY = y;
}

void InputManager::SetMouseDelta(glm::vec2 delta)
{
	mMouseDelta = delta;
}

void InputManager::Update()
{
	CalculateAxis();
	OnKeyHeld();
}

void InputManager::Clear()
{
	inputListeners.clear();
}


void InputManager::CalculateAxis()
{
	mAxisX = 0;
	mAxisY = 0;

	mAxisX += mRightHeld ? 1 : 0;
	mAxisX -= mLeftHeld ? 1 : 0;

	mAxisY += mUpHeld ? 1 : 0;
	mAxisY -= mDownHeld ? 1 : 0;
}


