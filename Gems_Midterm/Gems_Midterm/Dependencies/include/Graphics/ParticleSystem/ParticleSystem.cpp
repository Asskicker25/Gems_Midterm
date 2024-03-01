#include "ParticleSystem.h"
#include "ParticleSystemManager.h"
#include "../Panels/ImguiDrawUtils.h"
#include "../MathUtils.h"
#include "../Panels/EditorLayout.h"
#include "../Renderer.h"
#include "../UnlitColorMaterial.h"

using namespace MathUtilities;

System_Particle::ParticleSystem::ParticleSystem()
{
	isGizmoItem = true;
	name = "Particle System";


	LoadModel("res/Models/DefaultQuad.fbx", true);
	shader = Renderer::GetInstance().solidColorShader;

	meshes[0]->material = new UnlitColorMaterial();
	meshes[0]->material->AsUnlitMaterial()->alphaMask = new Texture("res/Textures/Icons/ParticleFXIcon.png");

	InitializeEntity(this);
	Renderer::GetInstance().AddModel(this);
	ParticleSystemManager::GetInstance().AddParticleSystem(this);

}

System_Particle::ParticleSystem::~ParticleSystem()
{
	ParticleSystemManager::GetInstance().RemoveParticleSystem(this);
}


void System_Particle::ParticleSystem::InitializeParticles()
{
	mListOfParticles.reserve(mMaxParticles);

	for (int i = 0; i < mMaxParticles; i++)
	{
		Particle newParticle;

		mListOfParticles.push_back(newParticle);
	}

}


void System_Particle::ParticleSystem::Play()
{
	isEnabled = true;
	mIsVisible = true;

	for (Particle& particle : mListOfParticles)
	{
		particle.lifeTime = 0;
	}
}

void System_Particle::ParticleSystem::Stop()
{
	isEnabled = false;
	mIsVisible = false;
}

void System_Particle::ParticleSystem::Pause()
{
	isEnabled = false;
}

void System_Particle::ParticleSystem::Resume()
{
	isEnabled = true;
}

void System_Particle::ParticleSystem::DrawProperty(std::string propertyName, ParticleSystemProperty& property)
{
	if (!ImGui::TreeNodeEx(propertyName.c_str()))
	{
		return;
	}

	property.OnPropertyDraw();

	ImGui::TreePop();
}

void System_Particle::ParticleSystem::OnPropertyDraw()
{
	Entity::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("Particle System", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}
	DrawSystemProperty();

	DrawProperty("Emission", mEmission);
	DrawProperty("Shape", mShape);
	DrawProperty("Renderer", mRenderer);

	ImGui::TreePop();


}

void System_Particle::ParticleSystem::DrawSystemProperty()
{
	if (!ImGui::TreeNodeEx("System"))
	{
		return;
	}

	ImGuiUtils::DrawBool("Enabled", isEnabled);
	ImGuiUtils::DrawBool("Visible", mIsVisible);
	ImGuiUtils::DrawBool("Looping", mIsLooping);

	ImGuiUtils::DrawInt("MaxParticles", mMaxParticles);

	ImGuiUtils::DrawFloat("CurrentTime", mCurrentTime);
	ImGuiUtils::DrawFloat("Duration", mDuration);
	ImGuiUtils::DrawFloat("StartDelay", mStartDelay);

	ImGuiUtils::DrawVector2ImGui("LifeTime", mStartLifeTime, 0, columnWidth);
	ImGuiUtils::DrawVector2ImGui("Velocity", mStartVelocity, 0, columnWidth);
	ImGuiUtils::DrawVector3ImGui("Size", mStartScale, 0, columnWidth);
	ImGuiUtils::DrawVector3ImGui("Rotation", mStartRotation, 0, columnWidth);
	ImGuiUtils::DrawVector3ImGui("Gravity", mGravity, 0, columnWidth);


	ImGui::TreePop();
}



void System_Particle::ParticleSystem::UpdateSystem(float deltaTime)
{

	//if (deltaTime > MAX_DELTATIME) { deltaTime = MAX_DELTATIME; }

	mCurrentTime += deltaTime;

	//if (mCurrentTime > mDuration) return;

	HandleRateOverTime(deltaTime);
	HandleBurst(deltaTime);
	HandleParticleMove(deltaTime);
}

void System_Particle::ParticleSystem::RenderSystem()
{
	if (!mIsVisible) return;

	for (Particle& particle : mListOfParticles)
	{
		if (particle.lifeTime <= 0) continue;

		mRenderer.mParticleModel->transform.SetPosition(particle.position);
		mRenderer.mParticleModel->transform.SetScale(particle.scale);
		mRenderer.mParticleModel->DrawShaded(mRenderer.mParticleModel->shader);
	}
}



void System_Particle::ParticleSystem::HandleRateOverTime(float deltaTime)
{
	if (mEmission.rateOverTime == 0) return;

	float currentRate = (mEmission.rateOverTime * deltaTime) + mRateOverTimeCarryOver;
	int numOfParticlesThisFrame = (int)currentRate;


	mRateOverTimeCarryOver = currentRate - numOfParticlesThisFrame;

	SpawnParticles(numOfParticlesThisFrame);

}

void System_Particle::ParticleSystem::HandleBurst(float deltaTime)
{
	for (Emission::Burst& burst : mEmission.mListOfBursts)
	{
		if (mCurrentTime <= burst.time) continue;
		if (burst.cycles != 0 && burst.currentCycle >= burst.cycles) continue;
		

		if (burst.currentCycle == 0)
		{
			SpawnBurstParticles(burst);
		}
		else
		{
			float currentTime = mCurrentTime - burst.time;
			int numOfBursts = (int)(currentTime / burst.interval);

			numOfBursts -= burst.currentCycle - 1;

			if (numOfBursts != 0)
			{
				SpawnBurstParticles(burst);
			}
		}

	}
}

void System_Particle::ParticleSystem::HandleParticleMove(float deltaTime)
{
	for (Particle& particle : mListOfParticles)
	{
		if (particle.lifeTime <= 0) continue;

		particle.lifeTime -= deltaTime;

		particle.velocity += mGravity * deltaTime;
		particle.position += particle.velocity * deltaTime;
	}
}

void System_Particle::ParticleSystem::SpawnBurstParticles(Emission::Burst& burst)
{
	float probability = MathUtils::GetRandomFloatNumber(0, 1.0f);

	if (probability < burst.probability)
	{
		SpawnParticles(burst.count);
		burst.currentCycle++;
	}
}


void System_Particle::ParticleSystem::SpawnParticles(int count)
{
	glm::vec3 pos;
	glm::vec3 dir;
	Particle* particle = nullptr;

	for (int i = 0; i < count; i++)
	{
		if (!GetDeadParticle(particle)) return;
		if (particle == nullptr) return;
		
		mShape.GetPositionAndDirection(pos, dir);

		particle->position = transform.position + pos;
		particle->velocity = dir * MathUtils::GetRandomFloatNumber(mStartVelocity.x, mStartVelocity.y);
		particle->lifeTime = MathUtils::GetRandomFloatNumber(mStartLifeTime.x, mStartLifeTime.y);
		particle->scale = mStartScale *  MathUtils::GetRandomFloatNumber(mRenderer.minParticleSize, mRenderer.maxParticleSize);

	}
}



bool System_Particle::ParticleSystem::GetDeadParticle(Particle*& outParticle)
{
	for (Particle& particle : mListOfParticles)
	{
		if (particle.lifeTime <= 0)
		{
			outParticle = &particle;
			return true;
		}
	}

	return false;
}

void System_Particle::ParticleSystem::Render()
{
	std::vector<Object*> selectedObjects = EditorLayout::GetInstance().GetSelectedObject();

	if (selectedObjects.size() == 0) return;

	if (selectedObjects[0] != this) return;


	mShape.Render(transform.position);
	
}