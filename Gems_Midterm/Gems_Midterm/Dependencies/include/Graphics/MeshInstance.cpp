#include "MeshInstance.h"
#include <glm/gtx/euler_angles.hpp>

void MeshInstance::AddTransformValues(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{

	glm::vec3 eulerAnglesRadians = glm::radians(rot);

	glm::quat quaternionRotation = glm::quat(
		glm::eulerAngleYXZ(eulerAnglesRadians.y, eulerAnglesRadians.x, eulerAnglesRadians.z));

	glm::mat4 rotation = glm::toMat4(quaternionRotation);

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos)
		* rotation
		* glm::scale(glm::mat4(1.0f), scale);


	mListOfModelMatrixes.push_back(modelMat);

}

void MeshInstance::DrawShaded(Shader* shader)
{
	meshes[0]->material->UpdateMaterial(shader);

	meshes[0]->mesh->VAO.Bind();

	GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	GLCALL(glDrawElements(GL_TRIANGLES, meshes[0]->mesh->IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

	const unsigned int numInstances = mListOfModelMatrixes.size();

	shader->SetUniformMatrix4fv("instancedModels", numInstances, GL_FALSE, glm::value_ptr(mListOfModelMatrixes[0]));

	GLCALL(glDrawElementsInstanced(GL_TRIANGLES, meshes[0]->mesh->IBO.GetCount(), GL_UNSIGNED_INT, nullptr, numInstances));

	meshes[0]->mesh->VAO.UnBind();
}

