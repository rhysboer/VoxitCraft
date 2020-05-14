#include "EntityPlayer.h"
#include "FPSCamera.h"
#include "Engine.h"
#include "Application.h"

EntityPlayer::EntityPlayer(const glm::vec3& position) : Entity(position) {
	camera = new FPSCamera(*this, glm::vec3(0, 0, 10));
	camera->SetFOV(60.0f);

	crosshair = new Sprite("./data/textures/crosshair.png");

	headOffset.y = 0.85f;
	size = glm::vec3(0.6f, 1.95f, 0.6f);
	currentBlock = (int)BlockIDs::DIRT;
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

	if(usePhysics == true) {
		direction.y = 0;
		this->Move(direction);
	} else {
		this->model->GetTransform().Translate(direction * 10.0f * Time::DeltaTime());
	}

	if(Input::IsKeyDown(GLFW_KEY_SPACE) && isGrounded) {
		this->SetGround(false);
		this->AddForce(0, 1.0f, 0, 0.15f);
	}

	// Place Block & Remove Block
	if(Input::IsMouseKeyPressed(GLFW_MOUSE_BUTTON_1)) {
		Ray::RayHit hit = Ray::RaycastFromCamera(10.0f, *camera);

		if(hit.hit) {
			if(AABB::IsOverlapping(this->GetPosition(), size, hit.neighbourPosition + glm::vec3(0.5f, 0.5f, 0.5f), glm::ivec3(1)) == false) {
				World::GetChunkManager().SetBlock(hit.neighbourPosition, (BlockIDs)currentBlock);
			}
		}
	} else if(Input::IsMouseKeyPressed(GLFW_MOUSE_BUTTON_2)) {
		Ray::RayHit hit = Ray::RaycastFromCamera(10.0f, *camera);

		if(hit.hit) {
			World::GetChunkManager().SetBlock(hit.hitPosition, BlockIDs::AIR);
		}
	}
	

	if(Input::IsKeyPressed(GLFW_KEY_3)) {
		World::GetChunkManager().FindChunk(this->GetPosition())->SetDirty();
	}

	if(Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		this->usePhysics = !this->usePhysics;

	if(Input::IsKeyDown(GLFW_KEY_Q))
		this->model->GetTransform().Translate(glm::vec3(0, 10.0f * Time::DeltaTime(), 0));
	if(Input::IsKeyDown(GLFW_KEY_E))
		this->model->GetTransform().Translate(glm::vec3(0, -10.0f * Time::DeltaTime(), 0));


	float scroll = Input::GetMouseScroll();
	if(scroll != 0.0f) {

		currentBlock += scroll;
		currentBlock = (currentBlock % (BlockManager::TotalBlocks() - 1));

		if(currentBlock <= 0)
			currentBlock = (scroll < 0) ? (BlockManager::TotalBlocks() - 2) : 1;

		printf("Current Block: %i\n", currentBlock);
	}


	ImGui::Begin("Info");
	ImGui::Text("Position: X:%f, Y:%f, Z:%f", model->GetPosition().x, model->GetPosition().y, model->GetPosition().z);
	ImGui::Text("Facing Direction: X:%i, Y:%i, Z:%i", (int)glm::round(camera->Forward().x), (int)glm::round(camera->Forward().y), (int)glm::round(camera->Forward().z));
	ImGui::Text("Chunk: %f, %f", glm::floor(model->GetPosition().x / 16.0f), glm::floor(model->GetPosition().z / 16.0f));
	ImGui::Text("Local Block: X:%i, Y:%i, Z:%i",
				Math::Modulo((int)glm::floor(model->GetPosition().x), Chunk::CHUNK_SIZE),
				Math::Modulo((int)glm::floor(model->GetPosition().y), Chunk::CHUNK_SIZE),
				Math::Modulo((int)glm::floor(model->GetPosition().z), Chunk::CHUNK_SIZE));
	ImGui::End();


	/*
		Vertex Order
			Z-
	 3rd _______ 2nd
		|\	    |
	 X- |  \    | X+
		|    \  |
	 4th|______\| 1st
		   Z+
		[5][4][3]
		[6][_][2]
		[7][0][1]
	*/


	// Crosshair
	glm::vec2 windowSize = Engine::GetWindowSize();
	ImGui::SetNextWindowPos(ImVec2((windowSize.x / 2.0f) - 17.0f, (windowSize.y / 2.0f) - 16.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::Begin("Crosshair", NULL , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
	ImGui::Image((void*)crosshair->GetTexture(), ImVec2(17, 17));
	ImGui::End();
	ImGui::PopStyleColor();

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

BlockIDs EntityPlayer::GetHeldBlock() {
	return (BlockIDs)currentBlock;
}
