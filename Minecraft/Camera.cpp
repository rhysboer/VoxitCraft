#include "Camera.h"
#include "Engine.h"

Camera* Camera::ActiveCamera;

Camera::Camera(float pos_x, float pos_y, float pos_z, float worldUp_x, float worldUp_y, float worldUp_z, float yaw, float pitch, float fov, float near, float far) {
	this->frustum = new Frustum();
	this->position = glm::vec3(pos_x, pos_y, pos_z);
	this->worldUp = glm::vec3(worldUp_x, worldUp_y, worldUp_z);
	this->yaw = yaw;
	this->pitch = pitch;
	this->fov = fov;
	this->_near = near;
	this->_far = far;

	ActiveCamera = this;
	isDirty = true;

	UpdateCamera();
}

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float fov, float near, float far) :
	Camera(position.x, position.y, position.z, worldUp.x, worldUp.y, worldUp.z, yaw, pitch, fov, near, far) {}

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float near, float far) : 
	Camera(position.x, position.y, position.z, worldUp.x, worldUp.y, worldUp.z, -90.0f, 0.0f, glm::radians(40.0f), near, far) {}

Camera::~Camera() {
	if(!frustum)
		delete frustum;

	frustum = nullptr;
}

void Camera::InputHandler() {
	glm::vec3 translate = glm::vec3(0.0f);

	float boost = 1.0f;
	if(Input::IsKeyDown(GLFW_KEY_W)) // FORWARD
		translate = this->forward;
	if(Input::IsKeyDown(GLFW_KEY_S)) // BACKWARD
		translate = -this->forward;
	if(Input::IsKeyDown(GLFW_KEY_D)) // RIGHT
		translate = this->right;
	if(Input::IsKeyDown(GLFW_KEY_A)) // LEFT
		translate = -this->right;
	if(Input::IsKeyDown(GLFW_KEY_E)) // UP
		translate = this->up;
	if(Input::IsKeyDown(GLFW_KEY_Q)) // DOWN
		translate = -this->up;
	if(Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) // Speed
		boost = CAMERA_BOOST;

	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_2)) { // RIGHT CLICK
		glm::vec2 deltaPos = Input::MouseDeltaPosition();
		Rotate(deltaPos.x, deltaPos.y);
	}

	position += (translate * Time::DeltaTime() * (CAMERA_SPEED * boost));
	UpdateCamera();
}

void Camera::Rotate(float x, float y) {
	yaw += x * CAMERA_SENSITIVITY;
	pitch -= y * CAMERA_SENSITIVITY;

	LimitRotation();
	isDirty = true;
}

void Camera::SetFar(const float& value) {
	_far = value;
	UpdateCamera();
}

void Camera::SetDirty() {
	isDirty = true;
}

glm::mat4 Camera::View() {
	DIRTY_CHECK;
	return view;
}

glm::mat4 Camera::Projection() {
	DIRTY_CHECK;
	return projection;
}

glm::mat4 Camera::ProjectionView() {
	DIRTY_CHECK;
	return projection * view;
}

glm::vec3 Camera::Up() {
	DIRTY_CHECK;
	return up;
}

glm::vec3 Camera::Right() {
	DIRTY_CHECK;
	return right;
}

glm::vec3 Camera::Forward() {
	DIRTY_CHECK;
	return forward;
}

glm::vec3 Camera::Position() {
	return position;
}

Frustum* Camera::GetFrustum() const {
	return frustum;
}

float Camera::GetFar() const {
	return _far;
}

float Camera::GetNear() const {
	return _near;
}

bool Camera::IsDirty() const {
	return isDirty;
}

void Camera::UpdateCamera() {
	glm::vec3 temp = glm::vec3(0.0f);
	temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch));
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(temp);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	view = glm::lookAt(position, position + forward, up);
	
	glm::vec2 size = Engine::GetWindowSize();
	projection = glm::perspective(fov, (size.y == 0) ? 0.0f : size.x / size.y, _near, _far);
	
	// Update camera frustum
	frustum->UpdateFrustum(*this);

	isDirty = false;
}

void Camera::LimitRotation() {
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
}

void Frustum::UpdateFrustum(const Camera& camera) {
	glm::mat4 inverseProjectionView = glm::inverse(camera.projection * camera.view);

	points.clear();
	points = {
		// NEAR
		glm::vec3(-1.0f, 1.0f,-1.0f), // TOP LEFT
		glm::vec3( 1.0f, 1.0f,-1.0f), // TOP RIGHT
		glm::vec3(-1.0f,-1.0f,-1.0f), // BOT LEFT
		glm::vec3( 1.0f,-1.0f,-1.0f), // BOT RIGHT
		// FAR
		glm::vec3(-1.0f, 1.0f, 1.0f), // TOP LEFT
		glm::vec3( 1.0f, 1.0f, 1.0f), // TOP RIGHT
		glm::vec3(-1.0f,-1.0f, 1.0f), // BOT LEFT
		glm::vec3( 1.0f,-1.0f, 1.0f)  // BOT RIGHT
	};

	glm::vec4 temp;
	for(int i = 0; i < 8; i++) {
		temp = inverseProjectionView * glm::vec4(points[i], 1.0f);
		points[i] = glm::vec3(temp) / temp.w;

		center += points[i];
	}

	center /= 8.0f;
}
