#pragma once

#include "../../Object.h"

namespace System_Particle
{
	class ParticleSystemProperty : public Object
	{
	public:

		virtual void OnPropertyDraw() = 0;
		virtual void OnSceneDraw() = 0;

	};
}