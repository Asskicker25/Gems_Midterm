#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::VertexBuffer(unsigned int size, const void* data)
{
	GLCALL(glGenBuffers(1, &rendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Setup(unsigned int size, const void* data)
{
	GLCALL(glGenBuffers(1, &rendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::UpdateVertexData(unsigned int size, const void* data)
{
	Bind();
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

