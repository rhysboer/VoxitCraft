#pragma once
#include <vector>
#include "Camera.h"

class Entity;

class EntityManager {
public:
	friend class Entity;

	static void Render(Camera& camera);
	static void Update();

private:

	static void AddEntity(Entity& entity);
	static void RemoveEntity(Entity& entity);

	static std::vector<Entity*> entities;
};

