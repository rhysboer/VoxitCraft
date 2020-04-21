#include "Application.h"

void Application::OnStart() {
	ShaderManager::InitShaders();

	camera = new Camera(glm::vec3(0, 0, 10));
	world = new World();
}

void Application::OnEnd() {
}

void Application::OnUpdate() {
	camera->InputHandler();

	if(Input::IsKeyPressed(GLFW_KEY_1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + isWireFrame);
		isWireFrame = !isWireFrame;
	}

	world->Update();
}

void Application::OnRender() {
	world->Render(*camera);
}
