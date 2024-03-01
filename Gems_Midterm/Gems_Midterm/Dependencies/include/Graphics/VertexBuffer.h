#pragma once
#include "Debugger.h"

class VertexBuffer
{
private:
	unsigned int rendererID;
	
public:
	VertexBuffer();
	VertexBuffer(unsigned int size , const void* data);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void Setup(unsigned int size, const void* data);
	void UpdateVertexData(unsigned int size, const void* data);
};

