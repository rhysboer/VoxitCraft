#pragma once
#include <vector>
#include "ParticleEmitter.h"
#include "glm/glm.hpp"

class ParticleSystem {
public:

	ParticleSystem();
	~ParticleSystem();

	void AddEmitter(ParticleEmitter* emitter);

	void Update();
	void RenderEmitters(BaseCamera& camera);

private:

	std::vector<ParticleEmitter*> emitters = std::vector<ParticleEmitter*>();
};

