#include "Transform.h"
#include <glm/gtx/euler_angles.hpp>
#include "Panels/ImguiDrawUtils.h"

Transform::Transform() : position{ glm::vec3(0) }, rotation{ glm::vec3(0) }, scale{ glm::vec3(1.0f) }
{
	UpdateQuaternionFromEuler();
}

Transform::Transform(const Transform& transform)
{
	position = transform.position;
	rotation = transform.rotation;
	scale = transform.scale;
}

void Transform::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void Transform::SetRotation(glm::vec3 _rotation)
{
	/*glm::vec3 delta = _rotation - rotation;
	rotation += delta;*/

	rotation = _rotation;
	UpdateQuaternionFromEuler();
}

void Transform::SetQuatRotation(glm::quat quatRotation)
{
	/*glm::quat delta = quatRotation - quaternionRotation;
	quaternionRotation += delta;*/

	quaternionRotation = quatRotation;
	UpdateEulerFromQuaternion();
}

void Transform::SetScale(glm::vec3 _scale)
{
	scale = _scale;
}


glm::mat4 Transform::GetTransformMatrix()
{
	//glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 rotation = glm::toMat4(quaternionRotation);

	return glm::translate(glm::mat4(1.0f), position)
		* rotation
		* glm::scale(glm::mat4(1.0f), scale);

	//trans = glm::translate(trans, position);

	///*glm::quat q = glm::quat(glm::eulerAngleX(glm::radians(90.0f)) *
	//	glm::eulerAngleY(glm::radians(90.0f)) *
	//	glm::eulerAngleZ(glm::radians(90.0f)));*/

	//trans *= 

	//trans = glm::scale(trans, scale);

	//return trans;
}

glm::mat4 Transform::GetInverseMatrix()
{
	return glm::inverse(glm::transpose(GetTransformMatrix()));
}

glm::vec3 Transform::GetForward()
{
	return glm::normalize(-glm::vec3(glm::mat4_cast(quaternionRotation)[2]));
	//return glm::normalize(-glm::vec3(GetTransformMatrix()[2]));
}

glm::vec3 Transform::GetUp()
{
	return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[1]));
	//return glm::normalize(glm::vec3(GetTransformMatrix()[1]));
}

glm::vec3 Transform::GetRight()
{
	return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[0]));
	//return glm::normalize(glm::vec3(GetTransformMatrix()[0]));
}


void Transform::SetUp(glm::vec3 newUp)
{
	newUp = glm::normalize(newUp);

	glm::vec3 axis = glm::cross(this->GetUp(), newUp);
	float angle = glm::acos(glm::dot(this->GetUp(), newUp));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetRight(glm::vec3 newRight)
{
	newRight = glm::normalize(newRight);

	glm::vec3 axis = glm::cross(this->GetRight(), newRight);
	float angle = glm::acos(glm::dot(this->GetRight(), newRight));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetForward(glm::vec3 newForward)
{
	newForward = glm::normalize(newForward);

	glm::vec3 axis = glm::cross(this->GetForward(), newForward);
	float angle = glm::acos(glm::dot(this->GetForward(), newForward));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetOrientationFromDirections(glm::vec3 newUp, glm::vec3 newRight)
{
	newUp = glm::normalize(newUp);
	newRight = glm::normalize(newRight);
	glm::vec3 newForward = glm::cross(newRight, newUp);

	glm::quat rotationQuat = glm::quatLookAt(newForward, newUp);

	this->SetQuatRotation(rotationQuat);
}


void Transform::UpdateQuaternionFromEuler()
{
	glm::vec3 eulerAnglesRadians = glm::radians(rotation);

	quaternionRotation =  glm::quat(
	glm::eulerAngleYXZ(eulerAnglesRadians.y, eulerAnglesRadians.x, eulerAnglesRadians.z));
}

void Transform::UpdateEulerFromQuaternion()
{
	rotation = glm::degrees(glm::eulerAngles(quaternionRotation));
}

void Transform::OnPropertyDraw()
{
	if (!ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawVector3ImGui("Position", position,0, posXColumnWidth);
	ImGuiUtils::DrawVector3ImGui("Rotation", rotation,0, rotXColumnWidth);
	ImGuiUtils::DrawVector3ImGui("Scale", scale, 1 , scaleXColumnWidth);

	SetRotation(rotation);

	ImGui::TreePop();

}

void Transform::OnSceneDraw()
{
}

