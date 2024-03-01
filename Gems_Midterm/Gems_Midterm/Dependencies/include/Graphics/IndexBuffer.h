#pragma once
#include "Debugger.h"

class IndexBuffer
{
private:
	unsigned int rendererId;
	unsigned int count;
public:
	IndexBuffer();
	IndexBuffer(unsigned int count, void* data);
	~IndexBuffer();

	void Bind() const;
	void UnBind();
	void Setup(unsigned int count, void* data);
	void UpdateBuffer(unsigned int count, void* data);

	inline int GetCount() const { return count; }
};

