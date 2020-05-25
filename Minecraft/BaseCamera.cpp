#include "BaseCamera.h"
#include "Engine.h"

BaseCamera::BaseCamera(const glm::vec3& _position, const glm::vec3& _worldUp, const float& _fov, const float& _near, const float& _far)
	: view(0), projection(0), forward(0), up(0), right(0), worldUp(_worldUp), yaw(-90.0f), pitch(0), fov(glm::radians(_fov)), near(_near), far(_far), isDirty(true)
{
	this->position = _position;
}

void BaseCamera::UpdateCamera() {

	glm::vec3 temp = glm::vec3(0.0f);
	temp.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch));
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(temp);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	view = glm::lookAt(position, position + forward, up);

	projection = CreateProjectionMatrix(); 

	// Update the cameras frustum planes
	UpdateFrustumPlanes();

	isDirty = false;
}

void BaseCamera::UpdateFrustumPlanes() {
	glm::mat4 projView = projection * view;

	// right side
	frustumPlanes[0] = glm::vec4(projView[0][3] - projView[0][0],
								 projView[1][3] - projView[1][0],
								 projView[2][3] - projView[2][0],
								 projView[3][3] - projView[3][0]);

	// left side
	frustumPlanes[1] = glm::vec4(projView[0][3] + projView[0][0],
								 projView[1][3] + projView[1][0],
								 projView[2][3] + projView[2][0],
								 projView[3][3] + projView[3][0]);
								 
	// top
	frustumPlanes[2] = glm::vec4(projView[0][3] - projView[0][1],
								 projView[1][3] - projView[1][1],
								 projView[2][3] - projView[2][1],
								 projView[3][3] - projView[3][1]);

	// bottom
	frustumPlanes[3] = glm::vec4(projView[0][3] + projView[0][1],
								 projView[1][3] + projView[1][1],
								 projView[2][3] + projView[2][1],
								 projView[3][3] + projView[3][1]);

	// far
	frustumPlanes[4] = glm::vec4(projView[0][3] - projView[0][2],
								 projView[1][3] - projView[1][2],
								 projView[2][3] - projView[2][2],
								 projView[3][3] - projView[3][2]);

	// near
	frustumPlanes[5] = glm::vec4(projView[0][3] + projView[0][2],
								 projView[1][3] + projView[1][2],
								 projView[2][3] + projView[2][2],
								 projView[3][3] + projView[3][2]);

	// plane normalisation, based on length of normal
	for(int i = 0; i < FRUSTUM_FACES; i++) {
		frustumPlanes[i] /= glm::length(glm::vec3(frustumPlanes[i]));
	}
}

bool BaseCamera::IsPointInFrustum(const glm::vec3& point) {
	DIRTY_CHECK;

	for(int i = 0; i < FRUSTUM_FACES; i++) {
		if(glm::dot(glm::vec3(frustumPlanes[i]), point) + frustumPlanes[i].w < 0.0f)
			return false;
	}

	return true;
}

bool BaseCamera::IsAABBInFrustum(const AABB& aabb) {
	DIRTY_CHECK;

	int out = 0;
	int in = 0;

	for(int i = 0; i < FRUSTUM_FACES; i++) {

		out = 0;
		in = 0;

		for(int v = 0; v < 8; v++) {
			if(glm::dot(glm::vec3(frustumPlanes[i]), aabb.GetPoint(v)) + frustumPlanes[i].w < 0.0f) {
				out++;
			} else {
				in++;
			}
		}

		if(in == 0)
			return false;
	}

	return true;
}


//bool BaseCamera::IsAABBInFrustum(glm::vec3 point) {
//	DIRTY_CHECK;
//
//	for(int i = 0; i < 6; i++) {
//		if(glm::dot(glm::vec3(frustumPlanes[i]), point) + frustumPlanes[i].w < 0.0f) {
//			return false;
//		}
//	}
//
//	return true;
//}


BaseCamera::BaseCamera(const BaseCamera& camera) {
	for(int i = 0; i < 6; i++)
		this->frustumPlanes[i] = camera.frustumPlanes[i];

	this->view = camera.view;
	this->projection = camera.projection;
	this->position = camera.position;
	this->forward = camera.forward;
	this->up = camera.up;
	this->right = camera.right;
	this->worldUp = camera.worldUp;

	this->yaw = camera.yaw;
	this->pitch = camera.pitch;
	this->fov = camera.fov;
	this->near = camera.near;	
	this->far = camera.far;
	this->isDirty = camera.isDirty;
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