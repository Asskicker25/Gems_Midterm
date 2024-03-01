#pragma once


#include "../Object.h"

#include "../Shaders/Shader.h"
#include "../FrameBuffer.h"

#include "../RenderQuad.h"

namespace PostProcessing_v1
{
	class BaseEffect : public Object
	{

	public:
		int stackOrder = 0;

		BaseEffect();

		virtual void OnPropertyDraw() = 0;
		virtual void OnSceneDraw() = 0;
		virtual void OnApplyEffect(FrameBuffer* sceneBuffer) = 0;

	};
}

