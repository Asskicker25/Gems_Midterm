#include "Texture.h"

void Texture::LoadImage(const char* path, Image& image)
{
	int width, height, channels;
	if (!fileExists(path))
	{
		//std::cout << "File does not exit";
	}
	unsigned char* imageData = stbi_load(path, &width, &height, &channels, 0);

	if (!imageData) {
		std::cerr << "Failed to load image at path: " << path << "\n"
			<< "Reason: " << stbi_failure_reason() << std::endl;

		//stbi_image_free(imageData);

		return;
	}

	image.width = width;
	image.height = height;
	image.pixelData = imageData;
	image.bpp = channels;

	if (channels == 1)
		image.format = GL_RED;
	else if (channels == 3)
		image.format = GL_RGB;
	else if (channels == 4)
		image.format = GL_RGBA;

	//stbi_image_free(imageData);

	//Debugger::Print("Image loaded successfully");

}

void Texture::LoadImage(const char* path, GLFWimage& image)
{
	Image _image;
	LoadImage(path, _image);

	image.height = _image.height;
	image.width = _image.width;
	image.pixels = _image.pixelData;
}

void Texture::OnPropertyDraw()
{
}

void Texture::OnSceneDraw()
{
}


Texture::Texture(const std::string& path)
{
	this->path = path;

	GLCALL(glGenTextures(1, &renderedID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, renderedID));

	LoadImage(path.c_str(), texture);

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.format == GL_RGBA ? GL_REPEAT : GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.format == GL_RGBA ? GL_REPEAT : GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0,texture.format , GL_UNSIGNED_BYTE, texture.pixelData));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &renderedID));
}

void Texture::Bind()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, renderedID));
}

void Texture::Unbind()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetTextureSlot(int slot)
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
}

void Texture::LoadTexture(const std::string& path)
{
	this->path = path;

	GLCALL(glGenTextures(1, &renderedID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, renderedID));

	LoadImage(path.c_str(), texture);

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.format == GL_RGBA ? GL_REPEAT : GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.format == GL_RGBA ? GL_REPEAT : GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, texture.pixelData));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
}
