#pragma once

#include "Object.h"
#include "Vertex.h"
#include "Shaders/Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "Texture/BaseTexture.h"
#include "IndexBuffer.h"
#include "TextureData.h"
#include "Material.h"
#include "Triangle.h"

class Mesh : public Object
{
public:
	std::vector<Vertex> vertices;
	std::vector< unsigned int> indices;
	std::vector< Triangles > triangles;
	std::vector< BaseTexture* > textures;

	Mesh() = default;
	Mesh(std::vector<Vertex>& vertices, std::vector< unsigned int>& indices, std::string name = "NoName");

	void DrawShadedMesh(Shader* shader, BaseMaterial* material,  bool isWireFrame = false);
	void DrawSolidColorMesh(Shader* shader, glm::vec3 color, bool isWireFrame = false);
	void DrawNormals(Shader* shader, glm::vec3 color, glm::mat4 transformMatrix);

	void UpdateVertices();
	void UpdateVertices(std::vector<Vertex>& vertices, std::vector< unsigned int> indices);

	VertexArray VAO;

	std::string name;

	static float normalScale;

private:

	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexLayout layout;

	//unsigned int VAO, VBO, EBO;

	void SetupMesh();
	void CalculateTriangles();

	// Inherited via Object
	void OnPropertyDraw() override;

	// Inherited via Object
	void OnSceneDraw() override;
};

