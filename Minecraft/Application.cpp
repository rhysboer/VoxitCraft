#include "Application.h"

Application* Application::app = nullptr;

Application& Application::Get() {
	return *app;
}

void Application::SetPostProcessEffect(const PostProcessEffect& effect) {
	app->effect = effect;
}

void Application::OnStart() {
	ShaderManager::InitShaders();

	app = this;

	effect = PostProcessEffect::NORMAL;
	camera = new Camera(glm::vec3(0, 0, 10));
	world = new World();
	lastResolution = Engine::GetWindowSize();


	postProcessing = new PostProcess("postProcess", 1280, 720, FramebufferType::COLOUR_AND_DEPTH);
	postProcessing->GetShader().SetTextureUnit("screenColour", 0);
	postProcessing->GetShader().SetTextureUnit("screenDepth", 1);

	framebuffer = new Framebuffer(glm::ivec2(1280, 720), FramebufferType::DEPTH);

	Input::DisableCursor(true);
}

void Application::OnEnd() {
	//delete postProcess_Water;
	//delete camera;
	//delete world;
}

void Application::OnUpdate() {
	// DEBUG
	//if(Input::IsKeyPressed(GLFW_KEY_1)) {
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + isWireFrame);
	//	isWireFrame = !isWireFrame;
	//}

	// DEBUG
	if(Input::IsKeyPressed(GLFW_KEY_2) || Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		lockCursor = !lockCursor;
		Input::DisableCursor(lockCursor);
	}

	// Update framebuffer resolution
	glm::vec2 resolution = Engine::GetWindowSize();
	if(lastResolution != resolution) {
		lastResolution = resolution;
		postProcessing->GetBuffer().SetSize(resolution);
	}
	
	world->Update();
}

void Application::OnRender() {
	// Render the world into the buffer to add post processing effects to it
	postProcessing->Begin();

	world->Render();
	Renderer::Render(*World::GetPlayer().Camera());



	postProcessing->End();
	
	// Set shader settings
	postProcessing->GetShader().SetInt("effectType", (int)effect);
	postProcessing->GetBuffer().BindTextures(0);

	// Render screen quad
	postProcessing->Render();
}