#pragma once
#include "BaseParticleShape.h"

namespace System_Particle
{
	class SphereParticleShape : public BaseParticleShape
	{
	public:

		float radius = 1;

		// 0 - 1. Where 1 is no inside sphere 0 is full sphere.
		float radiusThickness = 1;

		// Inherited via BaseParticleShape
		void OnPropertyDraw() override;
		void OnSceneDraw() override;
		void Render(glm::vec3& parentPos) override;

		void GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction) override;

		glm::vec3 minDir = glm::vec3(-1);
		glm::vec3 maxDir = glm::vec3(1);
		glm::vec4 outRadiusColor = glm::vec4(0,1,0,1);
		glm::vec4 inRadiusColor = glm::vec4(0,0,0,1);

	};
}


