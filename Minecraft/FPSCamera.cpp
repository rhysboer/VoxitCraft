#include "FPSCamera.h"
#include "Entity.h"

FPSCamera::FPSCamera(Entity& _entity, const glm::vec3& position) : BaseCamera(position) {
	this->entity = &_entity;
}

FPSCamera::~FPSCamera() {
}

void FPSCamera::Update() {
	//if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_2)) { // RIGHT CLICK
		glm::vec2 deltaPos = Input::MouseDeltaPosition();
		Rotate(deltaPos.x, deltaPos.y, 0.3f);

		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;
	//}

	position = entity->GetHeadPosition();
	UpdateCamera();
}
