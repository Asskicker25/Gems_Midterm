#pragma once

class Timer
{
public:
	static Timer& GetInstance();

	float deltaTime{ 0 };
	float currentTime{ 0 };
	float lastFrameTime{ 0 };
	float elapsedTime{ 0 };

	void SetCurrentTime(float currentTime);

	const float MAX_DELTATIME = 1.0f / 60.0f;

};

