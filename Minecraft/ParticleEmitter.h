#pragma once
#include "BlockManager.h"
#include "ShaderManager.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
};

struct ParticleVertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	float lightLevel;
};

class BaseCamera;

class ParticleEmitter {
public:

	ParticleEmitter(glm::vec3 position, const unsigned int& maxParticles, const BlockIDs& block);
	~ParticleEmitter();

	void Render(BaseCamera& camera);
	void Update();
	
	friend class ParticleSystem;

private:

	bool IsFinished() const;
	void CheckCollision(Particle& particle) const;

	const glm::vec3 gravity = glm::vec3(0, -1, 0);
	const float maxLife = 1.0;

	Shader* shader;

	Particle* particles;
	ParticleVertex* particleVertex;
	unsigned int* vertexIndexes;

	float currentLife;
	unsigned int totalParticles;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

