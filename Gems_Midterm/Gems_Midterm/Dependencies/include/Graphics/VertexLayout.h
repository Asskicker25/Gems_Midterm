#pragma once

#include "Debugger.h"

struct LayoutElement
{
	unsigned int count;
	unsigned int size;
	GLenum type;
	unsigned int offset;
};

class VertexLayout
{
private:

	unsigned int stride;
	unsigned int offset;

public:
	std::vector <LayoutElement> layoutElements;

	VertexLayout();
	~VertexLayout();

	template<typename T>
	void AddLayout(unsigned int count);
	
	
	void AssignLayouts() const;
	void Bind();
	void Unbind();

	inline unsigned int GetStride() const { return stride; }
};

