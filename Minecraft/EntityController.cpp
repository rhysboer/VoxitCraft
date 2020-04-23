#include "EntityController.h"

EntityController::~EntityController() {
}

void EntityController::RegisterEntity(Entity& entity) {
	this->entity = &entity;
}
