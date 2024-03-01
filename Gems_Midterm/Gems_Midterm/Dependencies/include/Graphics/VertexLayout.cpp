#include "VertexLayout.h"

VertexLayout::VertexLayout() : stride{ 0}, offset { 0}
{
}

VertexLayout::~VertexLayout()
{
	layoutElements.clear();
}

template<>
void VertexLayout::AddLayout<float>(unsigned int count)
{
	LayoutElement element;

	element.count = count;
	element.offset = offset;
	element.size = count * sizeof(float);
	element.type = GL_FLOAT;

	layoutElements.push_back(element);

	offset += element.size;
	stride = offset;
}

template<>
void VertexLayout::AddLayout<int>(unsigned int count)
{
	LayoutElement element;

	element.count = count;
	element.offset = offset;
	element.size = count * sizeof(int);
	element.type = GL_INT;

	layoutElements.push_back(element);

	offset += element.size;
	stride = offset;
}

void VertexLayout::AssignLayouts() const
{
	for (int i = 0; i < layoutElements.size(); i++)
	{
		GLCALL(glVertexAttribPointer(i, layoutElements[i].count, layoutElements[i].type, false, GetStride(), (void*)layoutElements[i].offset));
		GLCALL(glEnableVertexAttribArray(i));
	}
}

void VertexLayout::Bind()
{
	for (int i = 0; i < layoutElements.size(); i++)
	{
		GLCALL(glEnableVertexAttribArray(i));
	}
}

void VertexLayout::Unbind()
{
	for (int i = 0; i < layoutElements.size(); i++)
	{
		GLCALL(glDisableVertexAttribArray(i));
	}
}
