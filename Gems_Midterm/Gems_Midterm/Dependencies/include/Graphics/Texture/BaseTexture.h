#pragma once

#include "../Object.h"

class BaseTexture : public Object
{
public:
	
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void SetTextureSlot(int slot = 0) = 0;

	virtual void OnPropertyDraw() = 0;
	virtual void OnSceneDraw() = 0;
};

