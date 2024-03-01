#include "Model.h"
#include "Renderer.h"
#include "UnlitColorMaterial.h"
#include "Material.h"
#include "Panels/EditorLayout.h"

Model::Model()
{
	shader = Renderer::GetInstance().defaultShader;
}

Model::Model(const Model& model)
{
	isActive = model.isActive;
	meshes = model.meshes;
	directory = model.directory;
	transform = model.transform;
	shader = model.shader;
	//material = model.material;

}


Model::~Model()
{
	//std::cout << "Model ID : " << modelId <<std::endl;
	/*while (meshes.size() > 0)
	{
		if (meshes[0] != nullptr)
		{
			delete meshes[0];
		}
		meshes.erase(std::remove(meshes.begin(), meshes.end(), meshes[0]), meshes.end());
	}*/
	meshes.clear();
}

Model::Model(const std::string& path, bool debugModel)
{
	shader = Renderer::GetInstance().defaultShader;

	LoadModel(path, debugModel);
}

void Model::Draw(Shader* shader)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if (isWireframe)
		{
			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);
			continue;
		}

		switch (Renderer::GetInstance().renderMode)
		{
		case SHADED:

			DrawShaded(meshes[i], shader);

			break;
		case WIREFRAME:

			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);

			break;

		case SHADED_WIREFRAME:

			DrawShaded(meshes[i], shader);

			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);
			break;
		}

		/*	if (!renderer->showNormals) continue;

			DrawNormals(meshes[i], renderer->solidColorShader);*/
	}


}

void Model::DrawSolidColor(Shader* shader, glm::vec3 color)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		shader->Bind();
		SetModelMatrix(shader);

		meshes[i]->mesh->DrawSolidColorMesh(shader, color);

	}
}

void Model::SetModelParent(Model* model)
{
	parentModel = model;
}

void Model::DrawShaded(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();

	if (shader->applyModel)
	{
		SetModelMatrix(shader);
	}

	if (shader->applyInverseModel)
	{
		SetInverseModelMatrix(shader);
	}

	mesh->mesh->DrawShadedMesh(shader, mesh->material, false);
}

void Model::DrawWireframe(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();
	SetModelMatrix(shader);

	mesh->mesh->DrawSolidColorMesh(Renderer::GetInstance().solidColorShader,
		Renderer::GetInstance().wireframeMaterial->GetBaseColor(), true);
}


void Model::DrawNormals(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();
	SetModelMatrix(shader);

	mesh->mesh->DrawNormals(shader, Renderer::GetInstance().normalsMaterial->GetBaseColor(), transform.GetTransformMatrix());
}

void Model::SetModelMatrix(Shader* shader)
{
	shader->SetUniformMat("model",
		parentModel == nullptr ? transform.GetTransformMatrix() :
		parentModel->transform.GetTransformMatrix() * transform.GetTransformMatrix());
}

void Model::SetInverseModelMatrix(Shader* shader)
{
	shader->SetUniformMat("inverseModel",
		parentModel == nullptr ? transform.GetInverseMatrix() :
		parentModel->transform.GetInverseMatrix() * transform.GetInverseMatrix());
}


void Model::DrawNormals()
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		DrawNormals(meshes[i], Renderer::GetInstance().solidColorShader);
	}
}

void Model::DrawShaded(Shader* shader)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		DrawShaded(meshes[i], shader);
	}
}

void Model::DrawWireframe(const glm::vec3& color)
{
	Renderer::GetInstance().solidColorShader->Bind();
	SetModelMatrix(Renderer::GetInstance().solidColorShader);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->mesh->DrawSolidColorMesh(Renderer::GetInstance().solidColorShader, color, true);
	}

}

Model* Model::CopyFromModel(const Model& model, bool initialize)
{
	isActive = model.isActive;
	directory = model.directory;
	transform = model.transform;
	shader = model.shader;
	isGizmoItem = model.isGizmoItem;
	applyGizmoScale = model.applyGizmoScale;
	//material = model.material;

	for (MeshAndMaterial* mesh : model.meshes)
	{
		MeshAndMaterial* newMesh = new MeshAndMaterial();

		newMesh->mesh = mesh->mesh;
		newMesh->material = mesh->material->CloneMaterial();

		meshes.push_back(newMesh);
	}

	if (initialize)
	{
		InitializeEntity(this);
		Renderer::GetInstance().AddModel(this);
	}
	

	return this;
}


void Model::LoadModel(MeshDataHolder& meshData, bool debugModel)
{
	UnlitColorMaterial* meshMat = new UnlitColorMaterial();

	meshes.push_back(new MeshAndMaterial
		{ std::make_shared<Mesh>(meshData.vertices, meshData.indices), meshMat });


	if (debugModel) return;

	InitializeEntity(this);

}

Transform* Model::GetTransform()
{
	return &transform;
}


void Model::LoadModel(const std::string& path, bool debugModel)
{
	std::ifstream file(path);

	if (file.good())
	{
		//Debugger::Print("File exits");
	}
	else
	{
		//Debugger::Print("File doesn't exits");
	}

	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);


	if (debugModel) return;

	InitializeEntity(this);
	Renderer::GetInstance().AddModel(this);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//Mesh* myMesh = new Mesh(ProcessMesh(mesh, scene));
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

MeshAndMaterial* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 temp;

		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;

		vertex.positions = temp;

		if (mesh->HasNormals())
		{
			temp.x = mesh->mNormals[i].x;
			temp.y = mesh->mNormals[i].y;
			temp.z = mesh->mNormals[i].z;

			vertex.normals = temp;
		}

		/*if (mesh->mColors[0])
		{
			aiColor4D color = mesh->mColors[0][i];
			vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
		}
		else
		{
			vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}*/

		if (mesh->HasVertexColors(0))
		{
			aiColor4D color = mesh->mColors[0][i];
			vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
		}
		else
		{
			vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}


		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;

			/*temp.x = mesh->mTangents[i].x;
			temp.y = mesh->mTangents[i].y;
			temp.z = mesh->mTangents[i].z;

			vertex.tangents = temp;

			temp.x = mesh->mBitangents[i].x;
			temp.y = mesh->mBitangents[i].y;
			temp.z = mesh->mBitangents[i].z;*/

			//vertex.bitTangents = temp;

		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}




	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	BaseMaterial* meshMat;
	aiColor4D baseColor(1.0f, 1.0f, 1.0f, 1.0f);


	if (mesh->mMaterialIndex >= 0)
	{

		//aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &baseColor);

		meshMat = new Material();
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		meshMat->AsMaterial()->diffuseTexture = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		meshMat->AsMaterial()->specularTexture = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		meshMat->AsMaterial()->alphaMask = LoadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");

		if (((Texture*)meshMat->AsMaterial()->alphaMask)->path != "res/Textures/DefaultTextures/Opacity_Default.png")
		{
			meshMat->AsMaterial()->useMaskTexture = true;
		}

		meshMat->AsMaterial()->SetBaseColor(glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a));

		/*meshMat = new UnlitColorMaterial();
		meshMat->AsUnlitMaterial()->SetBaseColor(glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a));*/

	}
	else
	{
		meshMat = new UnlitColorMaterial();
		meshMat->AsUnlitMaterial()->SetBaseColor(glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a));
	}

	std::string meshName = mesh->mName.C_Str();

	if (meshName == "")
	{
		meshName = "Mesh " + std::to_string(meshes.size());
	}

	return new MeshAndMaterial{ std::make_shared<Mesh>(vertices, indices, meshName) , meshMat };
}


Texture* Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	//std::vector<Texture> textures;

	//Debugger::Print(typeName + " count : ", mat->GetTextureCount(type));

	//std::cout << mat->GetTextureCount(type) << std::endl;

	if (mat->GetTextureCount(type) == 0)
	{
		return LoadDefaultMaterialTextures(type, typeName);
	}


	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texString;

		mat->GetTexture(type, i, &texString);

		//Debugger::Print("Tex path ", texString.C_Str());

		//std::cout << "Tex path " << texString.C_Str() << std::endl;

		for (unsigned int i = 0; i < texturesLoaded.size(); i++)
		{
			if (std::strcmp(texturesLoaded[i]->path.data(), texString.C_Str()) == 0)
			{
				return texturesLoaded[i];
			}
		}
		std::string filename = std::string(texString.C_Str());
		filename = directory + '/' + filename;

		if (fileExists(filename))
		{
			Texture* texture = new Texture(filename);
			texture->path = texString.C_Str();
			texture->type = typeName;
			texturesLoaded.push_back(texture);

			return texture;
		}
		else
		{
			return LoadDefaultMaterialTextures(type, typeName);
		}
	}
}


Texture* Model::LoadDefaultMaterialTextures(aiTextureType type, std::string typeName)
{
	std::string path = "";
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		path = "res/Textures/DefaultTextures/Diffuse_Default.png";
		break;
	case aiTextureType_SPECULAR:
		path = "res/Textures/DefaultTextures/Specular_Default.jpg";
		break;
	case aiTextureType_OPACITY:
		path = "res/Textures/DefaultTextures/Opacity_Default.png";
		break;
	}
	Texture* texture = new Texture(path);

	texture->type = typeName;
	texture->path = path;
	texturesLoaded.push_back(texture);

	return texture;
}


void Model::Start()
{
}

void Model::Update(float deltaTime)
{
}

void Model::OnDestroy()
{
	Renderer::GetInstance().RemoveModel(this);
}

void Model::OnSceneDraw()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
	node_flags |= ImGuiTreeNodeFlags_SpanFullWidth;

	if (isSelected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::PushStyleColor(ImGuiCol_Text, isActive ? ImVec4(1,1,1,1) : ImVec4(0.4f,0.4,0.4,1));

	bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		OnModelSelected();
	}

	if (node_open)
	{
		for (MeshAndMaterial* mesh : meshes)
		{
			ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf;
			leaf_flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			leaf_flags |= ImGuiTreeNodeFlags_SpanFullWidth;

			if (mesh->mesh->isSelected)
			{
				leaf_flags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::TreeNodeEx(mesh->mesh->name.c_str(), leaf_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				Renderer::GetInstance().SetSelectedModel(this);
				EditorLayout::GetInstance().SetSelectedObjects({ mesh->mesh.get(), mesh->material });
			}
		}

		ImGui::TreePop();
	}

	ImGui::PopStyleColor();
}

void Model::OnPropertyDraw()
{
	ImGui::Checkbox("###ISActive", &isActive);
	ImGui::SameLine();
	Entity::OnPropertyDraw();
}


void Model::OnModelSelected()
{
	Renderer::GetInstance().SetSelectedModel(this);
	EditorLayout::GetInstance().SetSelectedObjects({ this });
}

void Model::Render()
{
}
