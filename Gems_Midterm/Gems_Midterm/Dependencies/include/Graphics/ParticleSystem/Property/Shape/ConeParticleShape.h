#pragma once
#include "BaseParticleShape.h"

namespace System_Particle
{
	class ConeParticleShape : public BaseParticleShape
	{
	public:

		float angle = 2;
		float radius = 1;
		float height = 1;

		enum EmitFrom
		{
			BASE = 0,
			VOLUME = 1,
		};

		void SetEmitFrom(EmitFrom emitFrom);
		EmitFrom GetEmitFrom();

		// Inherited via BaseParticleShape
		void OnPropertyDraw() override;
		void OnSceneDraw() override;
		void Render(glm::vec3& parentPos) override;

		void GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction) override;

	private:

		int currentEmitInt = 0;
		const char* emitStrings[2] = { "Base", "Volume" };

		float topRadius = 0;

		glm::vec3 endPos = glm::vec3(0);

		glm::vec4 bottomColor = glm::vec4(0, 1, 0, 1);
		glm::vec4 topColor = glm::vec4(0, 0, 1, 1);


		EmitFrom emitFrom = EmitFrom::BASE;

		float GetRadius(float& heightValue);
		glm::vec3 GetDirection(glm::vec3& up, glm::vec3& horizonal);
	};

}

