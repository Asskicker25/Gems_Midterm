#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(unsigned int indexCount, void* data) : count{ indexCount }
{
	GLCALL(glGenBuffers(1, &rendererId));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::UnBind()
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Setup(unsigned int indexCount, void* data) 
{
	count = indexCount;
	GLCALL(glGenBuffers(1, &rendererId));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::UpdateBuffer(unsigned int count, void* data)
{
	this->count = count;
	Bind();
	GLCALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
}
