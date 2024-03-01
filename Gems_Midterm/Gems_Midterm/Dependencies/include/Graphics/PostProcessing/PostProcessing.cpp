#include "PostProcessing.h"
#include "../Renderer.h"
#include "BaseEffect.h"

using namespace PostProcessing_v1;

void PostProcessing::Start()
{
}

void PostProcessing::Update(float deltaTime)
{
}

void PostProcessing::Render()
{
}

void PostProcessing::OnDestroy()
{
}

PostProcessing::PostProcessing(int width, int height)
{
	combineEffectsShader = new Shader("res/Shader/PostProcessing/CombineEffectsShader.shader", false, Shader::ALPHA_OPAQUE, false);

	bloom.InitializeFrameBuffer(width, height);
	bloom.stackOrder = 0;
	bloom.combineShader = combineEffectsShader;

	chromaticAberration.InitializeEffect("Chromatic Aberration", "res/Shader/PostProcessing/ChromaticAberration.shader");
	chromaticAberration.stackOrder = 1;
	chromaticAberration.combineShader = combineEffectsShader;

	heatDistortion.InitializeEffect("Heat Distortion", "res/Shader/PostProcessing/HeatDistortion.shader");
	heatDistortion.stackOrder = 2;
	heatDistortion.combineShader = combineEffectsShader;

	nightVision.InitializeEffect("Night Vision", "res/Shader/PostProcessing/NightVision.shader");
	nightVision.stackOrder = 3;
	nightVision.combineShader = combineEffectsShader;


	mListOfEffects.push_back(&bloom);
	mListOfEffects.push_back(&chromaticAberration);
	mListOfEffects.push_back(&heatDistortion);
	mListOfEffects.push_back(&nightVision);

	SortEffectsByOrder();

	name = "PostProcessing";
	InitializeEntity(this);

}

void PostProcessing::ApplyPostProcessing(FrameBuffer* sceneBuffer)
{

	for (BaseEffect* effect : mListOfEffects)
	{
		if (!effect->isEnabled) continue;

		effect->OnApplyEffect(sceneBuffer);
	}

}


void PostProcessing::OnPropertyDraw()
{

	for (BaseEffect* effect : mListOfEffects)
	{
		DrawEffectProperty(*effect);
	}
}

void PostProcessing::OnSceneDraw()
{
	//Entity::OnSceneDraw();
}


static bool CompareStackOrder(BaseEffect* a, BaseEffect* b)
{
	return a->stackOrder < b->stackOrder;
}

void PostProcessing::SortEffectsByOrder()
{
	std::sort(mListOfEffects.begin(), mListOfEffects.end(), &CompareStackOrder);
}

void PostProcessing::DrawEffectProperty(BaseEffect& effect)
{
	ImGui::Checkbox(("###" + std::to_string(effect.stackOrder)).c_str(), &effect.isEnabled);
	ImGui::SameLine();
	effect.OnPropertyDraw();
}

