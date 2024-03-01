#pragma once

#include "../Model.h"
#include "../EntityManager/Entity.h"
#include "Property/Emission.h"
#include "Property/ParticleRenderer.h"
#include "Property/Shape/ParticleShape.h"

namespace System_Particle
{
	class Particle
	{
	public:
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(0);

		glm::vec3 velocity = glm::vec3(0);

		float lifeTime = 0;

	};

	class ParticleSystem : public Model
	{
	public:
	
		ParticleSystem();
		~ParticleSystem();

		void Play();
		void Stop();

		void Pause();
		void Resume();

		void UpdateSystem(float deltaTime);
		void RenderSystem();
		void InitializeParticles();

		bool mIsVisible = true;
		bool mIsLooping = true;

		int mMaxParticles = 1000;

		float mDuration = 5;
		float mStartDelay = 0;
		float mCurrentTime = 0;

		glm::vec2 mStartLifeTime = glm::vec2(3,5);
		glm::vec2 mStartVelocity = glm::vec2(3, 5);

		glm::vec3 mStartScale = glm::vec3(1.0f);
		glm::vec3 mStartRotation = glm::vec3(0.8f);
		
		glm::vec3 mGravity = glm::vec3(0);


		Emission mEmission;
		ParticleShape mShape;
		ParticleRenderer mRenderer;


	private:

		float columnWidth = 150;
		float mRateOverTimeCarryOver = 0;
		const float MAX_DELTATIME = 1.0f / 60.0f;

		std::vector<Particle> mListOfParticles;

		// Inherited via Entity
		void Start() override {}
		void Update(float deltaTime) override {}
		void Render() override;
		void OnDestroy() override {}


		void DrawSystemProperty();
		void DrawProperty(std::string propertyName, ParticleSystemProperty& property);

		virtual void OnPropertyDraw();

		void HandleRateOverTime(float deltaTime);
		void HandleBurst(float deltaTime);
		void HandleParticleMove(float deltaTime);
		void SpawnParticles(int count);
		void SpawnBurstParticles(Emission::Burst& burst);

		bool GetDeadParticle(Particle*& outParticle);

	};
}


