#include "ParticleShape.h"
#include "../../../Panels/ImguiDrawUtils.h"
#include "SphereParticleShape.h"
#include "ConeParticleShape.h"
#include "BoxParticleShape.h"

System_Particle::ParticleShape::ParticleShape()
{
	AddShape(Shape::SPHERE, new SphereParticleShape());
	AddShape(Shape::CONE, new ConeParticleShape());
	AddShape(Shape::BOX, new BoxParticleShape());
}

void System_Particle::ParticleShape::SetShapeType(Shape shape)
{
	this->shape = shape;
	currentShapeItem = (int)shape;
}

System_Particle::BaseParticleShape* System_Particle::ParticleShape::GetShape()
{
	return mListOfShapes[shape];
}

void System_Particle::ParticleShape::AddShape(Shape shapeType, BaseParticleShape* shape)
{
	mListOfShapes[shapeType] = shape;
}

void System_Particle::ParticleShape::RemoveShape(Shape shapeType)
{
	mListOfShapes.erase(shapeType);
}

void System_Particle::ParticleShape::Render(glm::vec3& parentPos)
{
	GetShape()->Render(parentPos);
}

void System_Particle::ParticleShape::GetPositionAndDirection(glm::vec3& pos, glm::vec3& dir)
{
	GetShape()->GetPositionAndDirection(pos, dir);
}

void System_Particle::ParticleShape::OnPropertyDraw()
{
	if (ImGuiUtils::DrawDropDown("Shape", currentShapeItem, shapeString, 3))
	{
		SetShapeType((Shape)currentShapeItem);
	}
	mListOfShapes[shape]->OnPropertyDraw();

}

void System_Particle::ParticleShape::OnSceneDraw()
{
}

System_Particle::ConeParticleShape* System_Particle::ParticleShape::AsCone()
{
	return (ConeParticleShape*)GetShape();
}

System_Particle::SphereParticleShape* System_Particle::ParticleShape::AsSphere()
{
	return (SphereParticleShape*)GetShape();
}

System_Particle::BoxParticleShape* System_Particle::ParticleShape::AsBox()
{
	return (BoxParticleShape*)GetShape();
}
