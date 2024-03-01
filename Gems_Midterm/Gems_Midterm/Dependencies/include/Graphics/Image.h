#pragma once

#include "Debugger.h"

struct Image
{
	unsigned char* pixelData;
	int width;
	int height;
	int bpp;
	GLenum format;

	Image() : width{ 0 }, height{ 0 }, bpp{ 0 }, pixelData{ nullptr }
	{
	}

	
};

static bool fileExists(const std::string& path)
{
	std::ifstream file(path);
	return file.good();
}

