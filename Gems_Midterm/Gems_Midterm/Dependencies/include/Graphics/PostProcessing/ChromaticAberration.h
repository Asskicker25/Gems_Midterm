#pragma once

#include "SinglePassEffect.h"

namespace PostProcessing_v1
{
	class ChromaticAberration : public SinglePassEffect
	{
	public:
		// Inherited via SinglePassEffect
		void OnPropertyDraw() override;
		void SetShaderUniforms() override;

		float mAmount = 3;

	};
}


