#include "BaseCamera.h"
#include "Engine.h"

BaseCamera::BaseCamera(const glm::vec3& _position, const glm::vec3& _worldUp, const float& _fov, const float& _near, const float& _far)
	: view(0), projection(0), forward(0), up(0), right(0), worldUp(_worldUp), yaw(-90.0f), pitch(0), fov(glm::radians(_fov)), near(_near), far(_far), isDirty(true)
{
	this->position = _position;
}

void BaseCamera::UpdateCamera() {

	glm::vec3 temp = glm::vec3(0.0f);
	temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch));
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(temp);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	view = glm::lookAt(position, position + forward, up);

	projection = CreateProjectionMatrix(); 

	isDirty = false;
}

void BaseCamera::Rotate(const float& x, const float& y, const float& strength) {
	yaw += x * strength;
	pitch -= y * strength;

	isDirty = true;
}

void BaseCamera::SetNear(const float& distance) {
	near = distance;

	UpdateCamera();
}

void BaseCamera::SetFar(const float& distance) {
	far = distance;

	UpdateCamera();
}

void BaseCamera::SetFOV(const float& fov) {
	this->fov = glm::radians(fov);

	UpdateCamera();
}

void BaseCamera::SetDirty() {
	isDirty = true;
}

glm::mat4 BaseCamera::View() {
	DIRTY_CHECK;
	return view;
}

glm::mat4 BaseCamera::Projection() {
	DIRTY_CHECK;
	return projection;
}

glm::mat4 BaseCamera::ProjectionView() {
	DIRTY_CHECK;
	
	return projection * view;
}

glm::vec3 BaseCamera::Up() {
	DIRTY_CHECK;
	return up;
}

glm::vec3 BaseCamera::Right() {
	DIRTY_CHECK;
	return right;
}

glm::vec3 BaseCamera::Forward() {
	DIRTY_CHECK;
	return forward;
}

glm::vec3 BaseCamera::GetPosition() const {
	return position;
}

float BaseCamera::GetNear() const {
	return near;
}

float BaseCamera::GetFar() const {
	return far;
}
