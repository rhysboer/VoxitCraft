#pragma once
#include <cmath>
#include "Object3D.h"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"

class World;
class EntityManager;

class Entity {
public:
	friend class EntityManager;
	Entity(const glm::vec3& position);
	~Entity();

	glm::vec3 GetPosition() const;
	glm::vec3 GetHeadPosition() const;
	const bool IsGrounded() const;

	void SetPosition();
	void SetMovementSpeed(const float& speed);
	void SetGround(const bool& isGround);
	
	// Move entity by set amount, not affected by physics. 
	void Translate(const glm::vec3& position);
	void Translate(const float& x, const float& y, const float& z);

	// Add a direction force to the entity's velocity.
	void AddForce(const glm::vec3& direction, const float& force);
	void AddForce(const float& x, const float& y, const float& z, const float& force);

	// Move an entity in a normalized direction, multipled by its speed
	void Move(const glm::vec3& direction);
	void Move(const float& x, const float& y, const float& z);

protected:

	// Functions 
	void MoveEntity(const glm::vec3& velocity);

	virtual void OnUpdate();
	virtual void OnRender(BaseCamera& camera);
	virtual void OnCollision(const glm::vec3& normal);

	// Variables
	glm::vec3 headOffset;
	Object3D* model;
	float movementSpeed;
	bool usePhysics;
	bool isGrounded;

	glm::vec3 size;
	glm::vec3 velocity;
	glm::vec3 direction;
};

