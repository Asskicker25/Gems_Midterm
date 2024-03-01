#pragma once

#include "ParticleSystem.h"

namespace System_Particle
{
	class ParticleSystemManager
	{
	public:

		ParticleSystemManager() = default;

		static ParticleSystemManager& GetInstance();

		void AddParticleSystem(ParticleSystem* particleSystem);
		void RemoveParticleSystem(ParticleSystem* particleSystem);

		void Update(float deltaTime);
		void Render();

	private:
		std::vector<ParticleSystem*> mListOfParticleSystem;
	};
}

