#include "Engine.h"
#include <string>

GLFWwindow* Engine::window = nullptr;

void _CallbackFrameBufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool Engine::CreateEngine(const char* title, const int& width, const int& height) {
	if(!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title, NULL, NULL);
	
	if(!this->window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(this->window);
	

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return false;
	}

	glViewport(0, 0, width, height);
	InitCallbacks();
	this->isInitialized = true;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	return true;
}

void Engine::Run() {
	if(!isInitialized)
		return;

	glEnable(GL_CULL_FACE); // Enable Culling of Faces
	glCullFace(GL_BACK); // Cull Back Face
	glEnable(GL_DEPTH_TEST); // Depth Testing
	glEnable(GL_BLEND); // Transperent Blend
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Init();

	// Call On Start
	OnStart();

	while(!glfwWindowShouldClose(this->window)) {
		Time::Update();

		std::string title = "Demo ms: " + std::to_string(Time::DeltaTime());
		title += " fps: " + std::to_string(1.0f / Time::DeltaTime());
		glfwSetWindowTitle(window, title.c_str());

		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		OnUpdate();

		Input::Update();
		
		glClearColor(this->colourBackground.r, this->colourBackground.g, this->colourBackground.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OnRender();

		// IMGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	OnEnd();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

glm::ivec2 Engine::GetWindowSize() {
	glm::ivec2 size = glm::ivec2(0.0f);
	if(!window) return size;

	glfwGetWindowSize(window, &size.x, &size.y);
	return size;
}

GLFWwindow& Engine::GetWindow() {
	return *window;
}


// Private Functions

Engine::Engine() {
	this->isInitialized = false;
	this->window = nullptr;
	this->colourBackground = glm::vec3(0.0f);
}

glm::ivec2 Engine::WindowSize() const {
	glm::ivec2 size;
	glfwGetWindowSize(this->window, &size.x, &size.y);
	return size;
}

void Engine::Init() {
	ShaderManager::InitShaders();
}

void Engine::InitCallbacks() {
	Input::SetCallbacks(this->window);
	glfwSetFramebufferSizeCallback(this->window, _CallbackFrameBufferResize);
}
