#pragma once
#include "BaseParticleShape.h"

namespace System_Particle
{
	class BoxParticleShape : public BaseParticleShape
	{
	public:
		// Inherited via BaseParticleShape
		void OnPropertyDraw() override;
		void OnSceneDraw() override;

		void GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction) override;


		// Inherited via BaseParticleShape
		void Render(glm::vec3& parentPos) override;

	};

}

