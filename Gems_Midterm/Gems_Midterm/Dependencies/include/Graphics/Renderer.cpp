#include "Renderer.h"
#include "UnlitColorMaterial.h"
#include "DebugLineData.h"
#include "CameraSystem.h"

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

void Renderer::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	outlineMaterial = new Material();
	wireframeMaterial = new Material();
	normalsMaterial = new Material();

	outlineMaterial->SetBaseColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	wireframeMaterial->SetBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	normalsMaterial->SetBaseColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	line = new Model();

	DebugLineData lineData;
	lineData.SetStartPoint(glm::vec3(0));
	lineData.SetEndPoint(glm::vec3(1));

	line->LoadModel(lineData, true);
}

void Renderer::Clear()
{
	GLCALL(glClearColor(backGroundColor.x, backGroundColor.y, backGroundColor.z, 1.0f));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glStencilMask(0x00);
}

void Renderer::ClearModelList()
{
	nonBlendModelAndShaders.clear();
	blendModelAndShaders.clear();
}

void Renderer::AddModel(Model* model)
{
	if (model->shader->blendMode == Shader::ALPHA_OPAQUE || model->shader->blendMode == Shader::ALPHA_CUTOUT)
	{
		nonBlendModelAndShaders.push_back(model);
	}
	else if (model->shader->blendMode == Shader::ALPHA_BLEND)
	{
		blendModelAndShaders.push_back(model);
	}
}

void Renderer::AddModel(Model& model)
{
	if (model.shader->blendMode == Shader::ALPHA_OPAQUE || model.shader->blendMode == Shader::ALPHA_CUTOUT)
	{
		nonBlendModelAndShaders.push_back(&model);
	}
	else if (model.shader->blendMode == Shader::ALPHA_BLEND)
	{
		blendModelAndShaders.push_back(&model);
	}
}

void Renderer::RemoveModel(Model* model)
{

	for (Model* iterator : nonBlendModelAndShaders)
	{
		if (iterator == model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), iterator), nonBlendModelAndShaders.end());
			return;
		}
	}

	for (Model* iterator : blendModelAndShaders)
	{
		if (iterator == model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), iterator), nonBlendModelAndShaders.end());
			return;
		}
	}
}

void Renderer::RemoveModel(Model& model)
{

	for (Model* iterator : nonBlendModelAndShaders)
	{
		if (iterator  == &model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), iterator), nonBlendModelAndShaders.end());
			return;
		}
	}

	for (Model* iterator : blendModelAndShaders)
	{
		if (iterator == &model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), iterator), nonBlendModelAndShaders.end());
			return;
		}
	}
}

void Renderer::SetSelectedModel(Model* model)
{
	selectedModel = model;
}

void Renderer::SetBackgroundColor(const glm::vec3& backGroundColor)
{
	this->backGroundColor = backGroundColor;
}

void Renderer::Draw(bool viewport)
{
	glDepthFunc(GL_LEQUAL);
	skyBox->DrawShaded(skyBox->shader);
	glDepthFunc(GL_LESS);

	Model* tempSelectedModel = nullptr;

	gizmoScaleMultiplier = 1;

	glm::vec3 viewportPos = CameraSystem::GetInstance().viewportCamera->transform.position;
		

	for (unsigned int i = 0; i < nonBlendModelAndShaders.size(); i++)
	{
		if (selectedModel != nullptr && nonBlendModelAndShaders[i] == selectedModel)
		{
			tempSelectedModel = nonBlendModelAndShaders[i];
			continue;
		}

		if (nonBlendModelAndShaders[i]->isGizmoItem)
		{
			if (viewport)
			{
				if (nonBlendModelAndShaders[i]->applyGizmoScale)
				{
					glm::vec3 diff = viewportPos - nonBlendModelAndShaders[i]->transform.position;
					gizmoScaleMultiplier = glm::length(diff);
					gizmoScaleMultiplier *= gizmoScaleDownMultiplier;

					nonBlendModelAndShaders[i]->transform.SetScale(glm::vec3(gizmoIconSize) * gizmoScaleMultiplier);
				}
				nonBlendModelAndShaders[i]->Draw(nonBlendModelAndShaders[i]->shader);
			}

		}
		else
		{
			nonBlendModelAndShaders[i]->Draw(nonBlendModelAndShaders[i]->shader);
		}
	}

	SortBlendModels();

	for (Model* model : blendModelAndShaders)
	{
		if (selectedModel != nullptr && model == selectedModel)
		{
			tempSelectedModel = model;
			continue;
		}


		if (model->isGizmoItem)
		{
			if (viewport)
			{
				if (model->applyGizmoScale)
				{
					glm::vec3 diff = viewportPos - model->transform.position;
					gizmoScaleMultiplier = glm::length(diff);
					gizmoScaleMultiplier *= gizmoScaleDownMultiplier;

					model->transform.SetScale(glm::vec3(gizmoIconSize * gizmoScaleMultiplier));
				}
				model->Draw(model->shader);
			}

		}
		else
		{
			model->Draw(model->shader);
		}
	}


	if (tempSelectedModel != nullptr)

	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		if (tempSelectedModel->isGizmoItem)
		{
			if (viewport)
			{
				if (tempSelectedModel->applyGizmoScale)
				{
					glm::vec3 diff = viewportPos - tempSelectedModel->transform.position;
					gizmoScaleMultiplier = glm::length(diff);
					gizmoScaleMultiplier *= gizmoScaleDownMultiplier;

					tempSelectedModel->transform.SetScale(glm::vec3(gizmoIconSize * gizmoScaleMultiplier));
				}
				tempSelectedModel->Draw(tempSelectedModel->shader);
			}

		}
		else
		{
			tempSelectedModel->Draw(tempSelectedModel->shader);
		}

		if (!viewport) return;

		if (showNormals)
		{
			tempSelectedModel->DrawNormals();
		}

		DrawOutline();
	}

}

void Renderer::DrawOutline()
{

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);


	glm::vec3 currentScale = selectedModel->transform.scale;

	selectedModel->transform.scale.x += selectedModel->transform.scale.x * outlineScaleValue;
	selectedModel->transform.scale.y += selectedModel->transform.scale.y * outlineScaleValue;
	selectedModel->transform.scale.z += selectedModel->transform.scale.z * outlineScaleValue;


	selectedModel->DrawSolidColor(solidColorShader, outlineMaterial->GetBaseColor());

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);

	selectedModel->transform.SetScale(currentScale);
}

void Renderer::SortBlendModels()
{
	CompareDistances compareDistance(camera->transform.position);
	std::sort(blendModelAndShaders.begin(), blendModelAndShaders.end(), compareDistance);
}

const glm::vec3& Renderer::GetNormalsScale()
{
	return this->normalsScale;
}

void Renderer::SetNormalsLineScale(const glm::vec3& scale)
{
	this->normalsScale = scale;
}

void Renderer::DrawAABB(const modelAABB& aabb, glm::vec4 color, bool drawFromModel)
{
	glm::vec3 targetExtents = 0.5f * (aabb.max - aabb.min);
	glm::vec3 center = 0.5f * (aabb.max + aabb.min);

	Model* cube;

	if (drawFromModel)
	{
		cube = debugCubesModel->DrawDebugModel();
	}
	else
	{
		cube = debugCubesData->DrawDebugModel();
	}

	cube->transform.SetPosition(center);
	cube->transform.SetRotation(glm::vec3(0));
	cube->transform.SetScale(targetExtents);
	cube->DrawWireframe(color);
}

void Renderer::DrawCube(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale, const glm::vec4 color, bool drawFromModel)
{
	Model* cube;

	if (drawFromModel)
	{
		cube = debugCubesModel->DrawDebugModel();
	}
	else
	{
		cube = debugCubesData->DrawDebugModel();
	}
	cube->transform.SetPosition(pos);
	cube->transform.SetRotation(rot);
	cube->transform.SetScale(scale);
	cube->DrawWireframe(color);
}

void Renderer::DrawSphere(const glm::vec3 center, float radius, glm::vec4 color)
{
	Model* sphere = debugSpheres->DrawDebugModel();
	sphere->transform.SetPosition(center);
	sphere->transform.SetScale(glm::vec3(radius));
	sphere->DrawWireframe(color);
}

void Renderer::DrawLine(const glm::vec3 startPoint, const glm::vec3 endPoint, glm::vec4 color)
{
	line->meshes[0]->mesh->vertices[0].positions = startPoint;
	line->meshes[0]->mesh->vertices[1].positions = endPoint;
	line->meshes[0]->mesh->vertices[2].positions = endPoint;

	line->meshes[0]->mesh->UpdateVertices();

	line->DrawWireframe(color);

}

