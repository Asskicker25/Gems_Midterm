#pragma once

#include "Debugger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "DebugModels.h"

enum RenderMode
{
	SHADED = 0,
	WIREFRAME = 1,
	SHADED_WIREFRAME = 2, 
};

struct modelAABB
{
	modelAABB() {}
	modelAABB(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
	}
	glm::vec3 min;
	glm::vec3 max;
};

struct CompareDistances 
{
	const glm::vec3& cameraPos;

	explicit CompareDistances(const glm::vec3& cameraPosition) : cameraPos(cameraPosition) {}

	bool operator()(const Model* lhs, const Model* rhs) const 
	{
		glm::vec3 diff1 = cameraPos - lhs->transform.position;
		glm::vec3 diff2 = cameraPos - rhs->transform.position;

		return glm::dot(diff2, diff2) < glm::dot(diff1, diff1);
	}
};

class Renderer
{
private :
	
	std::vector<Model*> nonBlendModelAndShaders;
	std::vector<Model*> blendModelAndShaders;


	glm::vec3 backGroundColor = glm::vec3(0.1f, 0.3f, 0.4f);
	glm::vec3 normalsScale = glm::vec3(0.001f, 0.1f, 0.001f);

	float outlineScaleValue = 0.05f;
	float gizmoScaleMultiplier = 1;
	float gizmoScaleDownMultiplier = 0.3f;

public:

	Model* selectedModel;
	Shader* defaultShader;
	Shader* solidColorShader;
	Shader* alphaBlendShader;
	Shader* alphaCutOutShader;

	Camera* camera = nullptr;

	Material* outlineMaterial;
	Material* wireframeMaterial;
	Material* normalsMaterial;

	Model* skyBox;
	Model* line;

	DebugModels* debugCubesModel;
	DebugModels* debugCubesData;
	DebugModels* debugSpheres;
	DebugModels* debugLines;

	RenderMode renderMode = SHADED;

	bool showNormals = false;

	float gizmoIconSize = 0.1f;

	static Renderer& GetInstance();

	void Initialize();

	void Clear();
	void ClearModelList();
	void AddModel(Model* model);
	void AddModel(Model& model);
	void RemoveModel(Model* model);
	void RemoveModel(Model& model);
	void SetSelectedModel(Model* model);
	void SetBackgroundColor(const glm::vec3& backGroundColor);
	void Draw(bool viewport = false);
	void DrawOutline();
	void SortBlendModels();
	const glm::vec3&  GetNormalsScale();
	void SetNormalsLineScale(const glm::vec3& scale);
	void DrawAABB(const modelAABB& aabb, glm::vec4 color = glm::vec4(0.0,0.0,1.0,1.0), bool drawFromModel = true);
	void DrawCube(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale, const glm::vec4 color, bool drawFromModel = true);
	void DrawSphere(const glm::vec3 center, float radius, glm::vec4 color = glm::vec4(0.0, 0.0, 1.0, 1.0));
	void DrawLine(const glm::vec3 startPoint, const glm::vec3 endPoint, glm::vec4 color);
};

