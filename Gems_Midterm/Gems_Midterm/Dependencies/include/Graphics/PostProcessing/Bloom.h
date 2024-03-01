#pragma once

#include "BaseEffect.h"

namespace PostProcessing_v1
{

	class Bloom : public BaseEffect
	{
	public:


		Bloom() = default;

		// Inherited via BaseEffect
		void OnPropertyDraw() override;
		void OnSceneDraw() override;

		void InitializeFrameBuffer(int width , int height);
		void OnApplyEffect(FrameBuffer* sceneBuffer) override;
		unsigned int GetColorAttachmentId();

		float mAmount = 8;

		Shader* combineShader = nullptr;

	private:

		void ExtractBrightnessBuffer(FrameBuffer* sceneBuffer);
		void BlurBuffer(FrameBuffer* buffer);
		void CombineSceneAndBlur(FrameBuffer* sceneBuffer);

		FrameBuffer* blurFrameBuffer = nullptr;
		FrameBuffer* brightnessFrameBuffer = nullptr;

		Shader* brightnessExtractShader = nullptr;
		Shader* blurShader = nullptr;

	};
}

