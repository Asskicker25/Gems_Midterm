#include "ApplicationWindow.h"
#include "EntityManager/EntityManager.h"
#include "InputManager/InputManager.h"
#include "Shaders/ShaderSystem.h"
#include "CameraSystem.h"

using namespace System_Particle;

ApplicationWindow::ApplicationWindow()
{
	viewportCamera = new Camera();
	viewportCamera->isViewPortCamera = true;
	CameraSystem::GetInstance().viewportCamera = viewportCamera;
}

ApplicationWindow::~ApplicationWindow()
{

}



void ApplicationWindow::InitializeWindow(int windowWidth, int windowHeight)
{
	if (!glfwInit())
	{
		Debugger::Print("GlfW Init failed");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	window = glfwCreateWindow(this->windowWidth, this->windowHeight, "Model Viewer", NULL, NULL);

	//User Pointer
	glfwSetWindowUserPointer(window, this);

	//SetWindowIcon(window, "Assets/Textures/Icon/Icon.png");
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->SetViewportSize(window, width, height);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->GetCursorCallback(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->MouseHeldCallback(window, button, action, mods);
			app->MouseButtonCallback(window, button, action, mods);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));

			app->KeyCallBack(window, key, scancode, action, mods);
			app->InputCallback(window, key, scancode, action, mods);
		});


	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontGlobalScale = 2.0f;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//Debugger::Print("Failed to initialize GLAD");
	}
	else
	{
		//Debugger::Print("GLAD Initialized Successfully");
	}

	FrameBufferSpecification fbSpecs;

	fbSpecs.width = windowWidth;
	fbSpecs.height = windowHeight;
	fbSpecs.attachments = { FrameBufferTextureFormat::RGBA8 , FrameBufferTextureFormat::DEPTH24STENCIL8 };

	viewportFrameBuffer = new FrameBuffer(fbSpecs);
	gameSceneFrameBuffer = new FrameBuffer(fbSpecs);

	skyBox = new Model("res/Models/DefaultCube.fbx", true);
	skyBox->meshes[0]->material = new SkyBoxMaterial();
	skyBox->shader = &skyboxShader;

	SkyBoxMaterial* skyBoxMaterial = skyBox->meshes[0]->material->AsSkyBoxMaterial();

	skyBoxMaterial->skyBoxTexture->LoadTexture({
		"res/Textures/SkyBox/Right.jpg",
		"res/Textures/SkyBox/Left.jpg",
		"res/Textures/SkyBox/Up.jpg",
		"res/Textures/SkyBox/Down.jpg",
		"res/Textures/SkyBox/Front.jpg",
		"res/Textures/SkyBox/Back.jpg",
		});

	skyboxShader.LoadShader("res/Shader/SkyBox.shader", Shader::ALPHA_OPAQUE, false);
	skyboxShader.applyModel = false;
	//Debugger::Print("SkyboxShader  Id : ", skyboxShader.GetShaderId());

	solidColorShader.LoadShader("res/Shader/SolidColorShader.shader", Shader::ALPHA_OPAQUE, false);
	//Debugger::Print("SolidColorShader  Id : ", solidColorShader.GetShaderId());

	defShader.LoadShader("res/Shader/Shader.shader");
	//Debugger::Print("DefShader Shader Id : ", defShader.GetShaderId());
	defShader.applyInverseModel = true;

	alphaBlendShader.LoadShader("res/Shader/Shader.shader");
	//Debugger::Print("TranparentShader Shader Id : ", alphaBlendShader.GetShaderId());
	alphaBlendShader.blendMode = Shader::ALPHA_BLEND;
	alphaBlendShader.applyInverseModel = true;

	alphaCutOutShader.LoadShader("res/Shader/Shader.shader");
	//Debugger::Print("AlphaCutOutShader Shader Id : ", alphaCutOutShader.GetShaderId());
	alphaCutOutShader.blendMode = Shader::ALPHA_CUTOUT;
	alphaCutOutShader.applyInverseModel = true;

	Renderer::GetInstance().solidColorShader = &solidColorShader;
	Renderer::GetInstance().defaultShader = &defShader;
	Renderer::GetInstance().alphaBlendShader = &alphaBlendShader;
	Renderer::GetInstance().alphaCutOutShader = &alphaCutOutShader;

	Renderer::GetInstance().camera = viewportCamera;


	debugCubesModel = new DebugModels("res/Models/DefaultCube.fbx");
	debugCubesData = new DebugModels(cubeData);
	debugSpheres = new DebugModels("res/Models/DefaultSphere.fbx");
	debugLines = new DebugModels(lineData);

	Renderer::GetInstance().debugCubesModel = debugCubesModel;
	Renderer::GetInstance().debugCubesData = debugCubesData;
	Renderer::GetInstance().debugSpheres = debugSpheres;
	Renderer::GetInstance().debugLines = debugLines;

	Renderer::GetInstance().skyBox = skyBox;

	Renderer::GetInstance().Initialize();

	LightManager::GetInstance().AddShader(defShader);
	LightManager::GetInstance().AddShader(alphaBlendShader);
	LightManager::GetInstance().AddShader(alphaCutOutShader);



	viewportCamera->SetCameraWidth(windowWidth);
	viewportCamera->SetCameraHeight(windowHeight);

	viewportCamera->name = "Viewport Camera";
	viewportCamera->InitializeCamera();

}

void ApplicationWindow::EngineUpdate()
{
	Timer::GetInstance().SetCurrentTime(glfwGetTime());

	/*stopKeyCallback = applicationPlay;
	stopMouseCallback = applicationPlay;*/

	MoveCameraKeyBoard(window);
	ProcessInput(window);


	if (applicationPlay)
	{
		EntityManager::GetInstance().Update(Timer::GetInstance().deltaTime);
		InputManager::GetInstance().Update();
		ParticleSystemManager::GetInstance().Update(Timer::GetInstance().deltaTime);
		Scene::SceneManager::GetInstance().Update();
	}

	Update();
}

void ApplicationWindow::EngineRender()
{

	//Imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();

	PanelManager::GetInstance().Update(windowWidth, windowHeight);

	LightManager::GetInstance().RenderLight();

#pragma region Render Pipeline

	RenderForCamera(viewportCamera, viewportFrameBuffer, true);

	for (Camera* cam : CameraSystem::GetInstance().mListOfCameras)
	{
		if (cam->renderTexture == nullptr) continue;

		RenderForCamera(cam, cam->renderTexture->mFrameBuffer);
	}

	RenderForCamera(CameraSystem::GetInstance().GetMainCamera(), gameSceneFrameBuffer);
	//RenderForCamera(CameraSystem::GetInstance().GetMainCamera(), gameFinalFrameBuffer);


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#pragma endregion

	debugCubesModel->Clear();
	debugCubesData->Clear();
	debugSpheres->Clear();
	debugLines->Clear();
}

void ApplicationWindow::MainLoop()
{
	if (imGuiPanelEnable)
	{
		EditorLayout::GetInstance().application = this;
		EditorLayout::GetInstance().InitializeLayout();
	}

	SetUp();

	Timer::GetInstance().lastFrameTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		EngineUpdate();

		EngineRender();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void ApplicationWindow::RenderForCamera(Camera* camera, FrameBuffer* frameBuffer, bool viewport)
{
	if (camera == nullptr) return;

	frameBuffer->Bind();

	Renderer::GetInstance().Clear();

	view = camera->GetViewMatrix();


	for (Shader* shader : ShaderSystem::GetInstance().mListOfShaders)
	{
		shader->Bind();

		shader->SetProjection(camera->GetMatrix());
		shader->SetView(view);
		shader->SetViewPosition(camera->transform.position);

		shader->Unbind();
	}

	if (viewport)
	{
		EntityManager::GetInstance().Render();
		Render();
	}


	Scene::SceneManager::GetInstance().Render();
	Renderer::GetInstance().Draw(viewport);

	ParticleSystemManager::GetInstance().Render();

	if (camera->applyPostProcessing)
	{
		camera->postProcessing->ApplyPostProcessing(frameBuffer);
	}

	frameBuffer->UnBind();
}


void ApplicationWindow::SetWindowIcon(const std::string& path)
{
	GLFWimage images[1];

	Texture::LoadImage(path.c_str(), images[0]);

	glfwSetWindowIcon(window, 1, images);
}

void ApplicationWindow::SetBackgroundColor(const glm::vec3& color)
{
	Renderer::GetInstance().SetBackgroundColor(color);
}


void ApplicationWindow::GetCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::GetInstance().SetMousePos(xpos, ypos);

	currentMousePos.x = xpos;
	currentMousePos.y = ypos;

	mouseDeltaPos = currentMousePos - lastMousePos;
	mouseDeltaPos.y = -mouseDeltaPos.y;
	
	InputManager::GetInstance().SetMouseDelta(mouseDeltaPos);

	/*int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (xpos <= 0)
	{
		glfwSetCursorPos(window, width, ypos);
	}
	else if (xpos >= width - 1)
	{
		glfwSetCursorPos(window, 0, ypos);
	}*/

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;


	if (stopMouseCallback) return;
	if (!mouseHeld) return;

	MoveMouse();

	mouseCameraMove = true;

}


void ApplicationWindow::MoveMouse()
{
	if (stopMouseCallback) return;
	if (!mouseCameraMove) return;
	//if (lastMousePos.x == 0 && lastMousePos.y == 0) return;

	if (viewportCamera->transform.rotation.z == 180 || viewportCamera->transform.rotation.z == -180)
	{
		if (viewportCamera->transform.rotation.x > (89.0f + 180))	viewportCamera->transform.rotation.x = 89.0f + 180;
		if (viewportCamera->transform.rotation.x < -89.0f - 180)	viewportCamera->transform.rotation.x = -89.0f - 180;
	}
	else
	{
		if (viewportCamera->transform.rotation.x > 89.0f)	viewportCamera->transform.rotation.x = 89.0f;
		if (viewportCamera->transform.rotation.x < -89.0f)	viewportCamera->transform.rotation.x = -89.0f;
	}


	//std::cout << cameraYaw << std::endl;

	//std::cout << "Camera Yaw " << cameraYaw << std::endl;

	if (viewportCamera->transform.rotation.z == 180 || viewportCamera->transform.rotation.z == -180)
	{
		viewportCamera->transform.rotation.y += mouseDeltaPos.x * mouseSens;
		viewportCamera->transform.rotation.x += mouseDeltaPos.y * mouseSens;
	}
	else
	{
		viewportCamera->transform.rotation.y -= mouseDeltaPos.x * mouseSens;
		viewportCamera->transform.rotation.x += mouseDeltaPos.y * mouseSens;
	}

	viewportCamera->transform.SetRotation(viewportCamera->transform.rotation);

	//camera->SetCameraForward();
}

void ApplicationWindow::MoveCameraKeyBoard(GLFWwindow* window)
{
	if (stopKeyCallback) return;
	if (!mouseHeld) return;

	float speed = moveSpeed * Timer::GetInstance().deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		viewportCamera->transform.position += viewportCamera->transform.GetForward() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		viewportCamera->transform.position -= viewportCamera->transform.GetForward() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		viewportCamera->transform.position -= glm::cross(viewportCamera->transform.GetForward(), viewportCamera->transform.GetUp()) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		viewportCamera->transform.position += glm::cross(viewportCamera->transform.GetForward(), viewportCamera->transform.GetUp()) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		viewportCamera->transform.position -= viewportCamera->transform.GetUp() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		viewportCamera->transform.position += viewportCamera->transform.GetUp() * speed;
	}
}

void ApplicationWindow::SetViewportSize(GLFWwindow* window, int width, int height)
{
	/*windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);*/

	/*viewportFrameBuffer->Resize(width, height);
	gameSceneFrameBuffer->Resize(width, height);
	gameFinalFrameBuffer->Resize(width, height);*/
}

void ApplicationWindow::MouseHeldCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
	if (stopMouseCallback) return;

	if (action == GLFW_PRESS)
	{
		InputManager::GetInstance().OnMouseButtonPressed(button);
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::GetInstance().OnMouseButtonReleased(button);
	}

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS && EditorLayout::GetInstance().IsViewportHovered())
	{
		mouseHeld = true;
	}
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
	{
		mouseHeld = false;
		mouseCameraMove = false;
	}
}

void ApplicationWindow::InputCallback(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
	if (action == GLFW_PRESS)
	{
		InputManager::GetInstance().OnKeyPressed(key);
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::GetInstance().OnKeyReleased(key);
	}

}

