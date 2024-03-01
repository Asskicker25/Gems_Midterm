#pragma once

#include "../FrameBuffer.h"
#include "../EntityManager/Entity.h"

#include "Bloom.h"
#include "ChromaticAberration.h"
#include "HeatDistortion.h"
#include "NightVision.h"

namespace PostProcessing_v1
{
	class PostProcessing : public Entity
	{

	public:
		PostProcessing(int width, int height);

		void ApplyPostProcessing(FrameBuffer* sceneBuffer);

		// Inherited via Entity
		void Start() override;
		void Update(float deltaTime) override;
		void Render() override;
		void OnDestroy() override;

		virtual void OnPropertyDraw();
		virtual void OnSceneDraw();


		Bloom bloom;
		ChromaticAberration chromaticAberration;
		HeatDistortion heatDistortion;
		NightVision nightVision;

	private:

		void SortEffectsByOrder();
		void DrawEffectProperty(BaseEffect& effect);


		Shader* combineEffectsShader = nullptr;

		std::vector<BaseEffect*> mListOfEffects;

		
	};
}

