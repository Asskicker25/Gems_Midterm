#pragma once

#include <stbi/stb_image.h>
#include "../Image.h"
#include "../Object.h"
#include "BaseTexture.h"

class Texture : public BaseTexture
{
private:
	Image texture;

public:

	unsigned int renderedID;
	std::string type;
	std::string path;
	
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind() override;
	void Unbind() override;
	void SetTextureSlot(int slot = 0) override;
	void LoadTexture(const std::string& path);

	static void LoadImage(const char* path, Image& image);
	static void LoadImage(const char* path, GLFWimage& image);


	// Inherited via Object
	void OnPropertyDraw() override;
	void OnSceneDraw() override;

};

