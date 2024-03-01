#include "Mesh.h"
#include "Renderer.h"

float Mesh::normalScale = 0.5f;

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::string name)
{
	VAO.Bind();

	this->vertices = vertices;
	this->indices = indices;
	this->name = name;

	/*if (textures.size() != 0)
	{
		material.diffuseTexture = textures[0];
		material.specularTexture = textures[1];
	}*/

	//this->textures = textures;

	SetupMesh();
}


void Mesh::DrawShadedMesh(Shader* shader, BaseMaterial* material, bool isWireframe)
{
	material->UpdateMaterial(shader);

	VAO.Bind();

	if (isWireframe)
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
	else
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

	/*material->diffuseTexture->Unbind();
	material->specularTexture->Unbind();*/
	VAO.UnBind();

}

void Mesh::DrawSolidColorMesh(Shader* shader, glm::vec3 color, bool isWireFrame)
{
	shader->Bind();
	shader->SetUniform4f("solidColor", color.x, color.y, color.z,1);

	VAO.Bind();

	if (isWireFrame)
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
	else
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

	VAO.UnBind();
}

void Mesh::DrawNormals(Shader* shader, glm::vec3 color, glm::mat4 transformMatrix)
{
	shader->Bind();
	shader->SetUniform4f("solidColor", color.x, color.y, color.z,1);


	for (int i = 0; i < triangles.size(); i+=10)
	{
		glm::vec3 transformedCenter = transformMatrix * glm::vec4(triangles[i].center, 1.0f);
		glm::vec3 transformedNormal =glm::normalize( transformMatrix * glm::vec4(triangles[i].normal,0));
		/*Model* model = Renderer::GetInstance().debugCubesModel->DrawDebugModel();

		model->transform.SetScale(Renderer::GetInstance().GetNormalsScale());

		glm::vec3 pos = transformedCenter + (transformedNormal * model->transform.scale.y * normalScale);

		model->transform.SetPosition(pos); 

		model->transform.SetOrientationFromDirections(transformedNormal,
			transformMatrix * glm::vec4(triangles[i].tangent,0.0f));

		
		model->DrawSolidColor(shader, color);*/

		Renderer::GetInstance().DrawLine(transformedCenter, (transformedCenter +
			(transformedNormal * normalScale)), glm::vec4(color,1));

		//Renderer::GetInstance().DrawLine(glm::vec3(0), glm::vec3(10,0,0), glm::vec4(color, 1));
		//model->Draw(shader);
	}
}


void Mesh::UpdateVertices()
{
	VAO.Bind();
	VBO.UpdateVertexData(vertices.size() * sizeof(Vertex), &vertices[0]);
	IBO.UpdateBuffer(indices.size(), &indices[0]);
	VAO.AddBuffer(VBO, layout);
	VAO.UnBind();
}

void Mesh::UpdateVertices(std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	UpdateVertices();
}

void Mesh::SetupMesh()
{
	CalculateTriangles();

	VAO.Bind();
	VBO.Setup(vertices.size() * sizeof(Vertex), &vertices[0]);
	IBO.Setup(indices.size(), &indices[0]);

	//Position
	layout.AddLayout<float>(3);

	//Normals
	layout.AddLayout<float>(3);

	//Texture
	layout.AddLayout<float>(2);

	layout.AddLayout<float>(4);

	layout.AddLayout<float>(1);

	VAO.AddBuffer(VBO, layout);

	VAO.UnBind();
}

void Mesh::CalculateTriangles()
{
	if (indices.size() < 3) return;

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		Triangles triangle;
		triangle.v1 = vertices[indices[i]].positions;
		triangle.v2 = vertices[indices[i + 1]].positions;
		triangle.v3 = vertices[indices[i + 2]].positions;

		triangle.center = (triangle.v1 + triangle.v2 + triangle.v3) / 3.0f;

		triangle.normal = (vertices[indices[i]].normals +
			vertices[indices[i + 1]].normals +
			vertices[indices[i + 2]].normals) / 3.0f;

		glm::vec3 edge = triangle.v2 - triangle.v1;

		triangle.tangent = glm::normalize(glm::cross(triangle.normal, edge));

		triangles.push_back(triangle);
	}

}

void Mesh::OnPropertyDraw()
{
	ImGui::InputText("##ObjectName", &name[0], 516);
}

void Mesh::OnSceneDraw()
{
}



