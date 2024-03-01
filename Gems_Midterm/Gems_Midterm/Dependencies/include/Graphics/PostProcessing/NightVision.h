#pragma once

#include "SinglePassEffect.h"

namespace PostProcessing_v1
{
	class NightVision : public SinglePassEffect
	{
		// Inherited via SinglePassEffect
		void OnPropertyDraw() override;
		void SetShaderUniforms() override;
	};
}
