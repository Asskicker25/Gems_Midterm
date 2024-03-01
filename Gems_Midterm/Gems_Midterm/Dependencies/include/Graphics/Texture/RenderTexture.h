#pragma once

#include "../Debugger.h"
#include "../FrameBuffer.h"
#include "BaseTexture.h"

class RenderTexture : public BaseTexture
{
public:
	RenderTexture();

	void InitializeRenderTexture(FrameBufferSpecification specs);

	FrameBuffer* mFrameBuffer = nullptr;

	void Bind() override;
	void Unbind() override;
	void SetTextureSlot(int slot = 0) override;

	// Inherited via BaseTexture
	void OnPropertyDraw() override;
	void OnSceneDraw() override;
};

