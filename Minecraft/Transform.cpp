#include "Transform.h"

Transform::Transform() {
	//: position(0), scale(1), rotation(), isDirty(true)
	position = glm::vec3(0);
	scale = glm::vec3(1);
	rotation = glm::quat(1, 0, 0, 0);

	UpdateTransform();
}

Transform::~Transform() {
}

void Transform::SetPosition(const glm::vec3& position) {
	SetDirty();
	
	this->position = position;
}

void Transform::SetScale(const glm::vec3& scale) {
	SetDirty();

	this->scale = scale;
}

void Transform::SetScale(const float& scale) {
	SetDirty();

	this->scale = glm::vec3(scale);
}

void Transform::SetRotation(const glm::vec3& euler) {
	SetDirty();

	this->rotation = glm::quat(euler);
}

void Transform::RotateX(const float& angle) {
	SetDirty();

	this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(1, 0, 0));
}

void Transform::RotateY(const float& angle) {
	SetDirty();

	this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(0, 1, 0));
}

void Transform::RotateZ(const float& angle) {
	SetDirty();

	this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(0, 0, 1));
}

void Transform::RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis) {
	SetDirty();

	
	//glm::mat4 lookAt = glm::lookAt(position, glm::vec3(point.x, position.y, point.z), axis);
	//rotation = glm::toQuat(glm::inverse(lookAt));

	glm::vec4 direction = glm::vec4(position - point, 1.0);
	glm::vec4 rot = glm::rotate(glm::quat(1,0,0,0), glm::radians(angle), axis) * direction;
	position = rot + glm::vec4(point, 0.0);
}

void Transform::SetParent(Transform* parent) {
	SetDirty();

	this->parent = parent;
}

void Transform::Translate(const glm::vec3& direction) {
	SetDirty();
	
	this->position += direction;
}

void Transform::SetDirty() {
	isDirty = true;
}

glm::vec3 Transform::GetPosition() const {
	return position;
}

glm::mat4 Transform::GetTransform() {
	UpdateTransform();

	return transform;
}

void Transform::UpdateTransform() {
	transform = glm::translate(glm::mat4(1), position);
	transform *= glm::toMat4(rotation);
	transform = glm::scale(transform, scale);

	if(parent)
		transform = parent->GetTransform() * transform;

	isDirty = false;
}


