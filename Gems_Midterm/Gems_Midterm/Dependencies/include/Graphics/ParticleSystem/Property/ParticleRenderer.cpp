#include "ParticleRenderer.h"

System_Particle::ParticleRenderer::ParticleRenderer()
{
	mParticleModel = new Model("res/Models/DefaultSphere.fbx", true);
}

void System_Particle::ParticleRenderer::OnPropertyDraw()
{
}

void System_Particle::ParticleRenderer::OnSceneDraw()
{
}
