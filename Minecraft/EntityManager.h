#pragma once
#include <vector>
#include "BaseCamera.h"

class Entity;

class EntityManager {
public:
	friend class Entity;

	static void Render(BaseCamera& camera);
	static void Update();

private:

	static void AddEntity(Entity& entity);
	static void RemoveEntity(Entity& entity);

	static std::vector<Entity*> entities;
};

