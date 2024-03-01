#include "Camera.h"
#include "Panels/EditorLayout.h"
#include "Renderer.h"
#include "UnlitColorMaterial.h"
#include "Panels/ImguiDrawUtils.h"

Camera::Camera()
{
	this->cameraType = PERSPECTIVE;
	this->nearPlane = 0.1f;
	this->farPlane = 100.0f;
	this->fov = 45.0f;
	this->name = "Camera";

	isGizmoItem = true;

	transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	transform.SetOrientationFromDirections(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
}

void Camera::InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov)
{
	cameraType = _cameraType;
	cameraWidth = _cameraWidth;
	cameraHeight = _cameraHeight;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
	fov = _fov;

	item_current = cameraType == PERSPECTIVE ? 0 : 1;

	Resize(cameraWidth, cameraHeight);

	postProcessing = new PostProcessing_v1::PostProcessing(cameraWidth, cameraHeight);

	//InitializeEntity(this);

	if (isViewPortCamera) return;

	LoadModel("res/Models/DefaultQuad.fbx", true);
	shader = Renderer::GetInstance().solidColorShader;

	meshes[0]->material = new UnlitColorMaterial();
	//meshes[0]->material->AsUnlitMaterial()->diffuseTexture = new Texture("res/Textures/Icons/Light_Icon.png");
	meshes[0]->material->AsUnlitMaterial()->alphaMask = new Texture("res/Textures/Icons/Camera.png");
	Renderer::GetInstance().AddModel(this);



}

void Camera::InitializeCamera()
{
	Resize(cameraWidth, cameraHeight);
}

Camera::Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov) :
	cameraType{ _cameraType }, cameraWidth{ _cameraWidth }, cameraHeight{ _cameraHeight }, nearPlane{ _nearPlane }, farPlane{ _farPlane }, fov{ _fov }
{
	Resize(cameraWidth, cameraHeight);
	isGizmoItem = true;
}

Camera::Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane) :
	cameraType{ _cameraType }, cameraWidth{ _cameraWidth }, cameraHeight{ _cameraHeight }, nearPlane{ _nearPlane }, farPlane{ _farPlane }, fov{ 0 }
{
	Resize(cameraWidth, cameraHeight);
	isGizmoItem = true;
}

void Camera::Resize(float width, float height)
{
	aspectRatio = width / height;

	UpdateProjectionMatrix();

}

void Camera::UpdateProjectionMatrix()
{
	if (cameraType == PERSPECTIVE)
	{
		cameraMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}
	else
	{
		/*float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;
		cameraMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);*/
		cameraMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, nearPlane, farPlane);
	}
}

void Camera::ChangeCameraType(ECameraType type)
{
	cameraType = type;
	item_current = cameraType == PERSPECTIVE ? 0 : 1;
	Resize(cameraWidth, cameraHeight);
}

bool Camera::IsOrthographic()
{
	return cameraType == ORTHOGRAPHIC ? true : false;
}

void Camera::SetCameraForward()
{
	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(transform.rotation.y)) * glm::cos(glm::radians(transform.rotation.x));
	direction.y = glm::sin(glm::radians(transform.rotation.x));
	direction.z = glm::sin(glm::radians(transform.rotation.y)) * glm::cos(glm::radians(transform.rotation.x));

	transform.SetForward(glm::normalize(direction));
}

void Camera::SetCameraWidth(const float& width)
{
	this->cameraWidth = width;
}

void Camera::SetCameraHeight(const float& height)
{
	this->cameraHeight = height;
}

Transform* Camera::GetTransform()
{
	return &transform;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	return view;
}

void Camera::Start()
{
}

void Camera::Update(float deltaTime)
{
}

void Camera::OnDestroy()
{
}

void Camera::OnPropertyDraw()
{
	Entity::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	if (ImGuiUtils::DrawDropDown("Projection", item_current, items, 2))
	{
		cameraType = ECameraType(item_current);
			UpdateProjectionMatrix();
	};
	ImGui::TreePop();

		//ImGui::Columns(2);
		//ImGui::SetColumnWidth(0, 150);
		//ImGui::Text("Projection");
		//ImGui::NextColumn();

		//if (ImGui::Combo("###ProjectionType", &item_current, items, IM_ARRAYSIZE(items)))
		//{
		//	cameraType = ECameraType(item_current);
		//	UpdateProjectionMatrix();
		//}

		//ImGui::Columns(1);


		ImGui::Checkbox("###EnablePostProcess", &applyPostProcessing);
		ImGui::SameLine();

		if (!ImGui::TreeNodeEx("PostProcessing", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		postProcessing->OnPropertyDraw();

		ImGui::TreePop();
}

void Camera::OnSceneDraw()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;
	node_flags |= ImGuiTreeNodeFlags_SpanFullWidth;
	node_flags |= ImGuiTreeNodeFlags_Leaf;

	if (isSelected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		EditorLayout::GetInstance().SetSelectedObjects({ this });
		Renderer::GetInstance().SetSelectedModel(this);
	}
}
