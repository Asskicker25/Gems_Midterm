#pragma once

#include "../../../Object.h"

namespace System_Particle
{
	class BaseParticleShape : public Object
	{
		// Inherited via Object
	public:
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(0);

		bool randomizeDirection = false;
		
		virtual void GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction) = 0;

		virtual void OnPropertyDraw();
		virtual void OnSceneDraw() = 0;

		virtual void Render(glm::vec3& parentPos) = 0;

	private:
		float columnWidth = 150;
	};
}

