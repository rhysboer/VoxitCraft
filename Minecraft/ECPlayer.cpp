#include "ECPlayer.h"

ECPlayer::ECPlayer() {
}

ECPlayer::~ECPlayer() {
}

void ECPlayer::OnUpdate() {
	if(Input::IsKeyDown(GLFW_KEY_UP))
		entity->Move(0, 0, -1);
	if(Input::IsKeyDown(GLFW_KEY_DOWN))
		entity->Move(0, 0, 1);
	if(Input::IsKeyDown(GLFW_KEY_RIGHT))
		entity->Move(1, 0, 0);
	if(Input::IsKeyDown(GLFW_KEY_LEFT))
		entity->Move(-1, 0, 0);

	if(Input::IsKeyDown(GLFW_KEY_SPACE) && entity->IsGrounded()) {
		entity->SetGround(false);
		entity->AddForce(0, 1.0f, 0, 0.15f);
	}
}
