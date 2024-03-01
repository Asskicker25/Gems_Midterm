#pragma once

#include <random>
#include <glm/glm.hpp>

namespace MathUtilities
{
	class MathUtils
	{
	public:

		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale);
		static float Remap(float value, float inputMin, float inputMax, float outputMin, float outputMax);
		static const float GetRandomFloatNumber(float minValue, float maxValue);
		static const int GetRandomIntNumber(int minValue, int maxValue);
		static const glm::vec3 GetRandomVec3(glm::vec3 min, glm::vec3 max);
		static glm::vec3 Lerp(const glm::vec3& start, const glm::vec3& end, float t);
		static float Lerp(const float& start, const float& end, float t);
		static double CalculateTForSpeed(double currentT, double deltaTime, double lerpSpeed);
		static glm::vec3 GetRandomDirOnUnitCircle();
		static glm::vec3 GetRandomDirOnUnitCircle(glm::vec3& up);
	};

}

