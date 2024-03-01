#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "BaseMaterial.h"
#include "MeshDataHolder.h"
#include "EntityManager/Entity.h"
#include "Texture/Texture.h"

class Renderer;

struct MeshAndMaterial
{
	std::shared_ptr<Mesh> mesh;
	BaseMaterial* material;
};


class Model : public Entity
{
public: 
	Model();
	Model(const Model& model);
	~Model();
	Model(const std::string& path, bool debugModel = false);

	void Draw(Shader* shader);
	void DrawSolidColor(Shader* shader, glm::vec3 color);
	void SetModelParent(Model* model);
	Model* CopyFromModel(const Model& model, bool initialize = false);
	//Material material;

	Transform* GetTransform();
	void LoadModel(const std::string& path, bool debugModel = false);
	void LoadModel(MeshDataHolder& meshData, bool debugModel = false);
	void DrawNormals();
	void DrawShaded(Shader* shader);
	void DrawWireframe(const glm::vec3& color);

	virtual void OnPropertyDraw();
	virtual void OnSceneDraw();
	virtual void Render();

	// Inherited via Entity
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy();

	std::string modelId;
	bool isActive = true;
	bool isWireframe = false;
	bool isGizmoItem = false;
	bool applyGizmoScale = true;

	std::vector<MeshAndMaterial*> meshes;
	std::vector<Texture*> texturesLoaded;
	std::string directory;

	Shader* shader;

private:

	Model* parentModel = nullptr;

	void ProcessNode(aiNode* node, const aiScene* scene);
	MeshAndMaterial* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Texture* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Texture* LoadDefaultMaterialTextures(aiTextureType type, std::string typeName);

	void DrawShaded(MeshAndMaterial* mesh, Shader* shader);
	void DrawWireframe(MeshAndMaterial* mesh, Shader* shader);
	void DrawNormals(MeshAndMaterial* mesh, Shader* shader);

	void SetModelMatrix(Shader* shader);
	void SetInverseModelMatrix(Shader* shader);

	void OnModelSelected();

	
};

