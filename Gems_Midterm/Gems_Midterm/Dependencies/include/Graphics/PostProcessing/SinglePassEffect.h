#pragma once

#include "BaseEffect.h"

namespace PostProcessing_v1
{
	class SinglePassEffect : public BaseEffect
	{
	public:

		SinglePassEffect() { isEnabled = false; };
		void InitializeEffect(const std::string& name, const std::string& shaderPath);

		// Inherited via BaseEffect
		void OnPropertyDraw() = 0;
		void OnSceneDraw() {};
		void OnApplyEffect(FrameBuffer* sceneBuffer) override;

		virtual void SetShaderUniforms() = 0;

		Shader* shader = nullptr;
		Shader* combineShader = nullptr;
		FrameBuffer* frameBuffer = nullptr;

		std::string mName;
	};
}

