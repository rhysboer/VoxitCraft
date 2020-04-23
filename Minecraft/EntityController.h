#pragma once
#include "Entity.h"

class EntityController {
public:
	friend class Entity;

	~EntityController();

	virtual void OnUpdate() = 0;

protected:

	EntityController() {};
	Entity* entity = nullptr;

private:

	void RegisterEntity(Entity& entity);

};

