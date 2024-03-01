#include "FrameBuffer.h"

static bool IsDepthFormat(FrameBufferTextureFormat format)
{
	switch (format)
	{
	case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
	}

	return false;
}

static GLenum TextureTarget(bool multisampled)
{
	return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTexture(bool multisampled, uint32_t* outId, uint32_t count)
{
	GLCALL(glCreateTextures(TextureTarget(multisampled), count, outId));
}

static void BindTexture(bool multisampled, uint32_t id)
{
	GLCALL(glBindTexture(TextureTarget(multisampled), id));
}

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, 
	uint32_t width, uint32_t height, int index)
{
	bool multisampled = samples > 1;

	if (multisampled)
	{
		GLCALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE));
	}
	else
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr));

		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

	GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0));
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
{
	bool multisampled = samples > 1;
	if (multisampled)
	{
		GLCALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
	}
	else
	{
		GLCALL(glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height));

		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

	GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0));
}

FrameBuffer::FrameBuffer(FrameBufferSpecification specification)
{

	for (FrameBufferTextureSpecification spec : specification.attachments.mAttachments)
	{
		if (!IsDepthFormat(spec.mFormat))
		{
			colorAttachmentSpecs.emplace_back(spec);
		}
		else
		{
			depthAttachmentSpecs = spec;
		}
	}

	this->specification = specification;

	Validate();
}

FrameBuffer::~FrameBuffer()
{
	GLCALL(glDeleteFramebuffers(1, &rendererId));
	GLCALL(glDeleteTextures(colorAttachmentsID.size(), colorAttachmentsID.data()));
	GLCALL(glDeleteTextures(1, &depthAttachmentId));
}

void FrameBuffer::Validate()
{
	if (rendererId != 0)
	{
		GLCALL(glDeleteFramebuffers(1, &rendererId));
		GLCALL(glDeleteTextures(colorAttachmentsID.size(), colorAttachmentsID.data()));
		GLCALL(glDeleteTextures(1, &depthAttachmentId));

		colorAttachmentsID.clear();
		depthAttachmentId = 0;
	}

	GLCALL(glCreateFramebuffers(1, &rendererId));
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, rendererId));

	bool multisample = specification.samples > 1;

	if (colorAttachmentSpecs.size() > 0)
	{
		colorAttachmentsID.resize(colorAttachmentSpecs.size());
		CreateTexture(multisample, colorAttachmentsID.data(), colorAttachmentsID.size());
	
		for (int i = 0; i < colorAttachmentsID.size(); i++)
		{
			BindTexture(multisample, colorAttachmentsID[i]);
		
			switch (colorAttachmentSpecs[i].mFormat)
			{
			case FrameBufferTextureFormat::RGBA8:
				AttachColorTexture(colorAttachmentsID[i], specification.samples,
					GL_RGBA16F, GL_RGBA, specification.width, specification.height, i);
				break;
			}
		}
	}

	if (depthAttachmentSpecs.mFormat != FrameBufferTextureFormat::NONE)
	{
		CreateTexture(multisample, &depthAttachmentId, 1);
		BindTexture(multisample, depthAttachmentId);

		switch (depthAttachmentSpecs.mFormat)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8:
			AttachDepthTexture(depthAttachmentId, specification.samples,
				GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specification.width, specification.height);
			break;
		}
	}

	if (colorAttachmentSpecs.size() > 0)
	{
		if (colorAttachmentsID.size() <= 4)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			GLCALL(glDrawBuffers(colorAttachmentsID.size(), buffers));
		}
		else
		{
			__debugbreak();
		}
	}
	else if (colorAttachmentsID.empty())
	{
		glDrawBuffer(GL_NONE);
	}

	GLCALL(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));


}

void FrameBuffer::Bind()
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, rendererId));
	GLCALL(glViewport(0, 0, specification.width, specification.height));
}

void FrameBuffer::UnBind()
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width >= 6000 || height >= 6000) return;

	specification.width = width;
	specification.height = height;

	Validate();
}

unsigned int& FrameBuffer::GetColorAttachmentId(int index)
{
	if (index < colorAttachmentsID.size())
	{
		return colorAttachmentsID[index];
	}

	__debugbreak();
}

unsigned int& FrameBuffer::GetRendererId()
{
	return rendererId;
}

unsigned int& FrameBuffer::GetDepthAttachmentId()
{
	return depthAttachmentId;
}

FrameBufferSpecification& FrameBuffer::GetSpecification()
{
	return specification;
}

