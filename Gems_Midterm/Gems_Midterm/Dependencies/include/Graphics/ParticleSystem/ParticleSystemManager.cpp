#include "ParticleSystemManager.h"

using namespace System_Particle;

ParticleSystemManager& ParticleSystemManager::GetInstance()
{
	static ParticleSystemManager instance;
	return instance;
}

void ParticleSystemManager::AddParticleSystem(ParticleSystem* particleSystem)
{
	mListOfParticleSystem.push_back(particleSystem);
}

void ParticleSystemManager::RemoveParticleSystem(ParticleSystem* particleSystem)
{
	mListOfParticleSystem.erase(
		std::remove(mListOfParticleSystem.begin(), mListOfParticleSystem.end(), particleSystem) , mListOfParticleSystem.end());
}

void ParticleSystemManager::Update(float deltaTime)
{
	for (ParticleSystem* system : mListOfParticleSystem)
	{
		if (!system->isEnabled) continue;

		system->UpdateSystem(deltaTime);
	}
}

void ParticleSystemManager::Render()
{
	for (ParticleSystem* system : mListOfParticleSystem)
	{
		if (!system->mIsVisible) continue;

		system->RenderSystem();
	}
}
