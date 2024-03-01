#include "MazeApplication.h"

void MazeApplication::SetUp()
{
	viewportCamera->InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 500.0f, 65.0f);
	viewportCamera->transform.SetPosition(glm::vec3(127.4f, -147.6f, 242.4f));
	viewportCamera->transform.SetRotation(glm::vec3(0, 0, 0));
	viewportCamera->applyPostProcessing = true;

	EditorLayout::GetInstance().SetMaximizeState(true);

	moveSpeed = 100;

#pragma region Skybox

	skyBox->meshes[0]->material = new SkyBoxMaterial();
	SkyBoxMaterial* skyboxMat = skyBox->meshes[0]->material->AsSkyBoxMaterial();

	skyboxMat->skyBoxTexture->LoadTexture({
		"Assets/Textures/Skybox/Right.jpg",
		"Assets/Textures/Skybox/Left.jpg",
		"Assets/Textures/Skybox/Up.jpg",
		"Assets/Textures/Skybox/Down.jpg",
		"Assets/Textures/Skybox/Front.jpg",
		"Assets/Textures/Skybox/Back.jpg",
		});

#pragma endregion


	sceneOne = new Scene_One(this);

	SceneManager::GetInstance().AddScene("SceneOne", sceneOne);
	SceneManager::GetInstance().ChangeScene("SceneOne");

}

void MazeApplication::Update()
{
}

void MazeApplication::Render()
{
}

void MazeApplication::Shutdown()
{
}

void MazeApplication::ProcessInput(GLFWwindow* window)
{
}

void MazeApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
}

void MazeApplication::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}
