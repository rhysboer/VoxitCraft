#include "EntityManager.h"
#include "Entity.h"

std::vector<Entity*> EntityManager::entities = std::vector<Entity*>();

void EntityManager::Render(Camera& camera) {
	for(int i = 0; i < entities.size(); i++) {
		entities[i]->OnRender(camera);
	}
}

void EntityManager::Update() {
	for(int i = 0; i < entities.size(); i++) {
		entities[i]->OnUpdate();
	}
}

void EntityManager::AddEntity(Entity& entity) {
	entities.push_back(&entity);
}

void EntityManager::RemoveEntity(Entity& entity) {
	// TODO
}
