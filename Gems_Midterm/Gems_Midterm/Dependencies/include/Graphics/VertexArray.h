#pragma once
#include "Debugger.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
{
private:

public:
	unsigned int rendererID;
	VertexArray();
	~VertexArray();

	void Bind() const;
	void UnBind();
	void AddBuffer(const VertexBuffer& vb, const VertexLayout& layout);
};

