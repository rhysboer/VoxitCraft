#include "Entity.h"
#include "EntityManager.h"
#include "EntityController.h"
#include "World.h"

Entity::Entity(const char* modelPath, const glm::vec3& position, EntityController* controller) : usePhysics(true), isGrounded(false), movementSpeed(6), velocity(0), direction(0) {
	EntityManager::AddEntity(*this);
	
	model = Object3D::CreateObject_Cube(position);
	model->GetTransform().SetPosition(position);
	model->GetTransform().SetScale(0.5f);
	
	if(controller != nullptr)
		SetController(controller);
}

Entity::~Entity() {
	EntityManager::RemoveEntity(*this);

	// TODO: write model destroyer
	delete model;
}

void Entity::SetController(EntityController* controller) {
	this->controller = controller;
	this->controller->RegisterEntity(*this);
}

const glm::vec3& Entity::GetPosition() const {
	return model->GetPosition();
}

const bool Entity::IsGrounded() const {
	return isGrounded;
}

void Entity::SetGround(const bool& isGround) {
	this->isGrounded = isGround;
}

void Entity::Translate(const float& x, const float& y, const float& z) {
	model->GetTransform().Translate(glm::vec3(x, y, z));
}

void Entity::AddForce(const glm::vec3& direction, const float& force) {
	velocity += direction * force;
}

void Entity::AddForce(const float& x, const float& y, const float& z, const float& force) {
	AddForce(glm::vec3(x, y, z), force);
}

void Entity::OnUpdate() {
	if(controller)
		controller->OnUpdate();

	// Input Velocity
	if(direction != glm::vec3(0))
		MoveEntity((glm::normalize(direction) * Time::DeltaTime()) * movementSpeed);

	// Gravity
	velocity += (World::worldGravity / 2.0f) * Time::DeltaTime();
	MoveEntity(velocity);
	velocity += (World::worldGravity / 2.0f) * Time::DeltaTime();

	direction = glm::vec3(0);
}

void Entity::OnRender(Camera& camera) {
	model->Render(camera);
}

void Entity::Move(const glm::vec3& direction) {
	this->direction += direction;
}

void Entity::Move(const float& x, const float& y, const float& z) {
	Move(glm::vec3(x, y, z));
}

void Entity::MoveEntity(const glm::vec3& velocity) {
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
	int axes[3] = { 0, 2, 1 };

	int maxInterations = 10;
	int iterations = 0;

	glm::vec3 delta = velocity;

	for(int i = 0; i < 3; i++) {
		while(delta[axes[i]] != 0.0f) {
			float sign = glm::sign(delta[axes[i]]);
			float edge = model->GetPosition()[axes[i]] + sign * size[axes[i]] / 2.0f;

			float distToNext = (edge > 0.0f) ? 1.0f - glm::fract(edge) : glm::fract(edge);
			if(distToNext == 0.0f) distToNext = 1.0f;

			float offset = sign * glm::min(distToNext, glm::abs(delta[axes[i]]));

			glm::vec3 targetPos = model->GetPosition();
			targetPos[axes[i]] += offset;

			std::vector<glm::vec3> blocks = std::vector<glm::vec3>();
			World::GetChunkManager().GetSolidBlocksInArea(targetPos, size, blocks);

			if(blocks.size() == 0) {
				model->GetTransform().SetPosition(targetPos);
				delta[axes[i]] -= offset;
			} else {
				glm::vec3 axis = glm::vec3(0);
				axis[axes[i]] = -sign;

				OnCollision(axis);
				break;
			}

			if(iterations > maxInterations) {
				printf("Collsion Max Interation Reached\n");
				break;
			}

			++iterations;
		}

	}
}


void Entity::OnCollision(const glm::vec3& normal) {
	velocity.x = (normal.x != 0.0f) ? 0.0f : velocity.x;
	velocity.y = (normal.y != 0.0f) ? 0.0f : velocity.y;
	velocity.z = (normal.z != 0.0f) ? 0.0f : velocity.z;

	if(normal.y == 1.0f) {
		isGrounded = true;
		velocity.y = 0.0f;
	}
}
