#include "EntityPlayer.h"
#include "FPSCamera.h"
#include "Engine.h"
#include "Application.h"

EntityPlayer::EntityPlayer(const glm::vec3& position) : Entity(position)  {
	camera = new FPSCamera(*this, glm::vec3(0, 0, 10));
	crosshair = new Sprite("./data/textures/crosshair.png");

	headOffset.y = 0.85f;
	size = glm::vec3(0.6f, 1.95f, 0.6f);
}

EntityPlayer::~EntityPlayer() {
	delete camera;
	camera = nullptr;
}

void EntityPlayer::OnUpdate() {
	glm::vec3 forward = camera->Forward();
	glm::vec3 right = camera->Right();

	glm::vec3 direction = glm::vec3(0);

	if(Input::IsKeyDown(GLFW_KEY_W))
		direction += forward; 
	if(Input::IsKeyDown(GLFW_KEY_S))
		direction -= forward;
	if(Input::IsKeyDown(GLFW_KEY_D))
		direction += right; 
	if(Input::IsKeyDown(GLFW_KEY_A))
		direction -= right; 

	direction.y = 0;
	this->Move(direction);

	if(Input::IsKeyDown(GLFW_KEY_SPACE) && isGrounded) {
		this->SetGround(false);
		this->AddForce(0, 1.0f, 0, 0.15f);
	}

	glm::vec2 windowSize = Engine::GetWindowSize();

	// Crosshair
	ImGui::SetNextWindowPos(ImVec2((windowSize.x / 2.0f) - 17.0f, (windowSize.y / 2.0f) - 16.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::Begin("Crosshair", NULL , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
	ImGui::Image((void*)crosshair->GetTexture(), ImVec2(17, 17));
	ImGui::End();
	ImGui::PopStyleColor();

	printf("Position X:%f Y:%f Z:%f\n", GetPosition().x, GetPosition().y, GetPosition().z);
	if(World::GetChunkManager().GetBlock(GetHeadPosition()) == BlockIDs::WATER) {
		Application::SetPostProcessEffect(PostProcessEffect::UNDER_WATER);
	} else {
		Application::SetPostProcessEffect(PostProcessEffect::NORMAL);
	}

	Entity::OnUpdate();
	camera->Update();
}

FPSCamera* EntityPlayer::Camera() {
	return camera;
}
