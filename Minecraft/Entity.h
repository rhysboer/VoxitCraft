#pragma once
#include "Object3D.h"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"

class Entity {
public:

	virtual ~Entity() = 0;

	const glm::vec3& GetPosition() const;


protected:

	// Functions 

	Entity(const char* modelPath, glm::vec3 position);

	virtual void Update();
	void Render();


	// Variables

	Object3D model;

	glm::vec3 position;
};

