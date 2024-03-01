#include "SphereParticleShape.h"
#include "../../../Panels/ImguiDrawUtils.h"
#include "../../../MathUtils.h"
#include "../../../Renderer.h"

void System_Particle::SphereParticleShape::OnPropertyDraw()
{
	ImGuiUtils::DrawFloat("Radius", radius);
	ImGuiUtils::DrawDragFloat("R Thickness", radiusThickness,0.01f,0,1);
	BaseParticleShape::OnPropertyDraw();
}

void System_Particle::SphereParticleShape::OnSceneDraw()
{
}

void System_Particle::SphereParticleShape::GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction)
{
	direction = MathUtilities::MathUtils::GetRandomVec3(minDir, maxDir);
	direction = glm::normalize(direction);

	float randomLength = MathUtilities::MathUtils::GetRandomFloatNumber(radius - (radius * radiusThickness) , radius);

	pos = position + direction * randomLength;

	if (randomizeDirection)
	{
		direction = MathUtilities::MathUtils::GetRandomVec3(minDir, maxDir);
	}
}

void System_Particle::SphereParticleShape::Render(glm::vec3& parentPos)
{
	glm::vec3 center = parentPos + position;

	if (radiusThickness == 0)
	{
		Renderer::GetInstance().DrawSphere(center, radius - (radius * radiusThickness), inRadiusColor);
		return;
	}
	else
	{
		Renderer::GetInstance().DrawSphere(center, radius, outRadiusColor);
		Renderer::GetInstance().DrawSphere(center, radius - (radius * radiusThickness), inRadiusColor);
	}

	
}

