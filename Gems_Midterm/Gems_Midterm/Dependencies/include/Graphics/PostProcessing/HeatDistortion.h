#pragma once

#include "SinglePassEffect.h"

namespace PostProcessing_v1
{
	class HeatDistortion : public SinglePassEffect
	{

	public:

		// Inherited via SinglePassEffect
		void OnPropertyDraw() override;
		void SetShaderUniforms() override;

		float mAmount = 2;
	};
}

