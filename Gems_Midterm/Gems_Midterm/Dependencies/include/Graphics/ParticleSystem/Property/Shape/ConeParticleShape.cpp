#include <glm/gtx/euler_angles.hpp>

#include "ConeParticleShape.h"
#include "../../../Panels/ImguiDrawUtils.h"
#include "../../../MathUtils.h"
#include "../../../Renderer.h"

using namespace MathUtilities;

void System_Particle::ConeParticleShape::OnPropertyDraw()
{
	ImGuiUtils::DrawFloat("Angle", angle);
	ImGuiUtils::DrawFloat("Radius", radius);
	ImGuiUtils::DrawFloat("Height", height);


	if (ImGuiUtils::DrawDropDown("EmitFrom", currentEmitInt, emitStrings, 2))
	{
		SetEmitFrom((ConeParticleShape::EmitFrom)currentEmitInt);
	}

	BaseParticleShape::OnPropertyDraw();
}

void System_Particle::ConeParticleShape::OnSceneDraw()
{
}

void System_Particle::ConeParticleShape::GetPositionAndDirection(glm::vec3& pos, glm::vec3& direction)
{
	float randomHeight = 0;

	//Calculate Up based on rotation 
	glm::vec3 eulerAnglesRadians = glm::radians(rotation);
	glm::quat quatRot = glm::quat(
		glm::eulerAngleYXZ(eulerAnglesRadians.y, eulerAnglesRadians.x, eulerAnglesRadians.z));
	glm::vec3 up = glm::normalize(glm::vec3(glm::mat4_cast(quatRot)[1]));

	endPos = position + up * height;

	if (emitFrom == BASE)
	{
		pos = position;
	}
	else
	{
		randomHeight = MathUtils::GetRandomFloatNumber(0, height);

		pos = MathUtils::Lerp(position, endPos, randomHeight);
	}

	float radius = GetRadius(randomHeight);
	
	glm::vec3 dirOnCircle = MathUtils::GetRandomDirOnUnitCircle(up);
	dirOnCircle = glm::normalize(dirOnCircle);

	pos = pos + dirOnCircle * (MathUtils::GetRandomFloatNumber(0, radius));
	direction = GetDirection(up, dirOnCircle);
}

void System_Particle::ConeParticleShape::SetEmitFrom(EmitFrom emitFrom)
{
	this->emitFrom = emitFrom;
	currentEmitInt = (int)emitFrom;
}

System_Particle::ConeParticleShape::EmitFrom System_Particle::ConeParticleShape::GetEmitFrom()
{
	return this->emitFrom;
}

float System_Particle::ConeParticleShape::GetRadius(float& heightValues)
{
	return (radius + glm::tan(glm::radians(angle)) * heightValues);
}

glm::vec3 System_Particle::ConeParticleShape::GetDirection(glm::vec3& up, glm::vec3& horizontal)
{
	glm::vec3 normalizedUp = glm::normalize(up);
	glm::vec3 normalizedHorizontal = glm::normalize(horizontal);

	glm::vec3 rotationAxis = glm::cross(normalizedUp, normalizedHorizontal);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotationAxis);

	glm::vec3 direction = glm::vec3(rotationMatrix * glm::vec4(normalizedUp, 1.0f));

	return direction;
}


void System_Particle::ConeParticleShape::Render(glm::vec3& parentPos)
{
	glm::vec3 center = parentPos + position;
	glm::vec3 end =  parentPos + endPos;

	topRadius = GetRadius(height);

	Renderer::GetInstance().DrawSphere(center, radius, bottomColor);
	Renderer::GetInstance().DrawSphere(end + parentPos, topRadius, bottomColor);
	
}

