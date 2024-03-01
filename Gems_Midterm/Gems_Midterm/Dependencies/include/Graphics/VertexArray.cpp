#include "VertexArray.h"

VertexArray::VertexArray() 
{
	GLCALL(glGenVertexArrays(1, &rendererID));
	GLCALL(glBindVertexArray(rendererID));

}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(rendererID));
}

void VertexArray::UnBind()
{
	GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexLayout& layout)
{
	vb.Bind();
	layout.AssignLayouts();
}

