#include "ParticleSystem.h"
#include "BaseCamera.h"

ParticleSystem::ParticleSystem() {
	ShaderManager::AddShader("particle");
}

ParticleSystem::~ParticleSystem() {
	int size = emitters.size();
	for(int i = 0; i < size; i++) {
		delete emitters[i];
	}
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter) {
	if(emitter == nullptr) return;
	emitters.push_back(emitter);
}

void ParticleSystem::Update() {
	for(int i = 0; i < emitters.size(); i++) {
		emitters[i]->Update();

		if(emitters[i]->IsFinished()) {
			delete emitters[i];
		
			emitters.erase(emitters.begin() + i);
			--i;
		}
	}
}

void ParticleSystem::RenderEmitters(BaseCamera& camera) {
	glDisable(GL_CULL_FACE);
	for(int i = 0; i < emitters.size(); i++) {
		emitters[i]->Render(camera);
	}
	glEnable(GL_CULL_FACE);
}
