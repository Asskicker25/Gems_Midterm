#pragma once

#include "Debugger.h"

enum class FrameBufferTextureFormat
{
	NONE = 0,

	//Color 
	RGBA8,

	//Depth
	DEPTH24STENCIL8,

	DEPTH = DEPTH24STENCIL8,
};

struct FrameBufferTextureSpecification
{

	FrameBufferTextureSpecification() = default;
	FrameBufferTextureSpecification(FrameBufferTextureFormat format)
		: mFormat{ format } {}

	FrameBufferTextureFormat mFormat = FrameBufferTextureFormat::NONE;
};

struct FrameBufferAttachmentSpecification
{
	FrameBufferAttachmentSpecification() = default;
	FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
		: mAttachments{ attachments } {};
	
	std::vector<FrameBufferTextureSpecification> mAttachments;
};

struct FrameBufferSpecification
{
	uint32_t width = 0, height = 0;
	uint32_t samples = 1;

	FrameBufferAttachmentSpecification attachments;

	bool swapChainTarget = false;
};

class FrameBuffer
{

public:

	FrameBuffer(FrameBufferSpecification specifcation);
	~FrameBuffer();

	void Validate();
	
	void Bind();
	void UnBind();

	void Resize(uint32_t width, uint32_t height);

	unsigned int& GetColorAttachmentId(int index = 0);
	unsigned int& GetRendererId();
	unsigned int& GetDepthAttachmentId();

	FrameBufferSpecification& GetSpecification();

private:

	unsigned int rendererId = 0;
	unsigned int depthAttachmentId = 0;

	std::vector<unsigned int> colorAttachmentsID;
	std::vector<FrameBufferTextureSpecification> colorAttachmentSpecs;
	FrameBufferTextureSpecification depthAttachmentSpecs = FrameBufferTextureFormat::NONE;


	FrameBufferSpecification specification;

};

