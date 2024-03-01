#include "SkyBoxShader.h"

void SkyBoxShader::SetView(glm::mat4 matrix)
{
	glm::mat4 view = glm::mat4(glm::mat3(matrix));
	SetUniformMat("view", view);

}

void SkyBoxShader::SetViewPosition(glm::vec3 pos)
{
}

