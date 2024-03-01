#pragma once
#include "../ParticleSystemProperty.h"
#include "BaseParticleShape.h"

namespace System_Particle
{
	class ConeParticleShape;
	class BoxParticleShape;
	class SphereParticleShape;

	class ParticleShape : public ParticleSystemProperty
	{
	public:
		ParticleShape();

		enum Shape
		{
			SPHERE = 0,
			CONE = 1,
			BOX = 2
		};

		void SetShapeType(ParticleShape::Shape shape);
		Shape GetShapeType() { return shape; }

		BaseParticleShape* GetShape();

		void AddShape(Shape shapeType, BaseParticleShape* shape);
		void RemoveShape(ParticleShape::Shape shapeType);

		void Render(glm::vec3& parentPos);

		void GetPositionAndDirection(glm::vec3& pos, glm::vec3& dir);

		// Inherited via ParticleSystemProperty
		void OnPropertyDraw() override;
		void OnSceneDraw() override;

		ConeParticleShape* AsCone();
		SphereParticleShape* AsSphere();
		BoxParticleShape* AsBox();

	private:

		const char* shapeString[3] = {"Sphere", "Cone", "Box"};

		int currentShapeItem = 0;
		Shape shape = Shape::SPHERE;

		std::unordered_map< Shape, BaseParticleShape*> mListOfShapes;


	};
}


