#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{
	GLCALL(glGenTextures(1, &rendererID));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID));
}

void CubeMapTexture::Bind()
{
    GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID));
}

void CubeMapTexture::UnBind()
{
    GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMapTexture::SetTextureSlot(int slot)
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
}

void CubeMapTexture::LoadTexture(std::vector<std::string> faces)
{

    int width, height, channels;

    for (unsigned int i = 0; i < faces.size(); i++)
    {

        if (!fileExists(faces[i]))
        {
            std::cout << "File does not exit";
        }

        unsigned char* imageData = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        
        if (!imageData)
        {
            std::cerr << "Failed to load image at path: " << faces[i] << "\n"
                << "Reason: " << stbi_failure_reason() << std::endl;

            //stbi_image_free(imageData);
            return;
        }
        
        GLCALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData));
        
        //stbi_image_free(imageData);
    }

    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
   
}
