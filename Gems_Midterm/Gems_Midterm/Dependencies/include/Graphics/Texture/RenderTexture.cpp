#include "RenderTexture.h"

RenderTexture::RenderTexture()
{
}

void RenderTexture::InitializeRenderTexture(FrameBufferSpecification specs)
{
	mFrameBuffer = new FrameBuffer(specs);
}

void RenderTexture::Bind()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, mFrameBuffer->GetColorAttachmentId()));
}

void RenderTexture::Unbind()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void RenderTexture::SetTextureSlot(int slot)
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
}

void RenderTexture::OnPropertyDraw()
{
}

void RenderTexture::OnSceneDraw()
{
}
