#pragma once

class iInputListener
{
public:

	virtual ~iInputListener() {};

	virtual void OnKeyPressed(const int& key) {}
	virtual void OnKeyReleased(const int& key) {}
	virtual void OnKeyHeld(const int& key) {};
	virtual void OnMouseButtonPressed(const int& key) {}
	virtual void OnMouseButtonReleased(const int& key) {}
};
