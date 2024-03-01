#pragma once

#include "Image.h"
#include <stbi/stb_image.h>


class CubeMapTexture
{

public:
	unsigned int rendererID;

	CubeMapTexture();
	
	void Bind();
	void UnBind();
	void SetTextureSlot(int slot = 0);
	void LoadTexture(std::vector<std::string> faces);
};

