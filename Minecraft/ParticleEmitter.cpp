#include "ParticleEmitter.h"
#include "Time.h"
#include "BaseCamera.h"
#include "World.h"

ParticleEmitter::ParticleEmitter(glm::vec3 position, const unsigned int& maxParticles, const BlockIDs& block) : totalParticles(maxParticles) {
	// Data of each particle
	particles = new Particle[maxParticles];

	// Particle vertices
	particleVertex = new ParticleVertex[maxParticles * 4];

	// Vertex Indexes
	vertexIndexes = new unsigned int[maxParticles * 6];

	shader = ShaderManager::GetShader("particle");

	std::array<glm::vec2, 4> coords = std::array<glm::vec2, 4>();
	BlockManager::GetTextureCoords(block, 0, coords);

	for(int i = 0; i < maxParticles; i++) {
		float size = 1.0f / 12.0f;
		glm::vec3 posOffset = glm::vec3(((rand() % 100) / 100.0f) - 0.5, ((rand() % 100) / 100.0f) - 0.5, ((rand() % 100) / 100.0f) - 0.5);
		
		particles[i].position = posOffset + position;
		particles[i].velocity = glm::vec3(((rand() % 100) / 100.0f) - 0.5, ((rand() % 100) / 100.0f) - 0.5, ((rand() % 100) / 100.0f) - 0.5);

		float offsetX = 0.125f / 6.0f;
		float offsetY = 0.125f / 6.0f;
		glm::vec2 texTopLeft = coords[2];

		texTopLeft.x += offsetX * ((rand() % 5) + 1);
		texTopLeft.y -= offsetY * ((rand() % 5) + 1);

		particleVertex[(i * 4) + 0].texCoords = glm::vec2(texTopLeft.x, texTopLeft.y - offsetY);			// 1 | 0,1
		particleVertex[(i * 4) + 1].texCoords = glm::vec2(texTopLeft.x, texTopLeft.y);						// 2 | 0,0
		particleVertex[(i * 4) + 2].texCoords = glm::vec2(texTopLeft.x + offsetX, texTopLeft.y);			// 3 | 1,0
		particleVertex[(i * 4) + 3].texCoords = glm::vec2(texTopLeft.x + offsetX, texTopLeft.y - offsetY);	// 0 | 1,1

		vertexIndexes[(i * 6) + 0] = (i * 4) + 0;
		vertexIndexes[(i * 6) + 1] = (i * 4) + 1;
		vertexIndexes[(i * 6) + 2] = (i * 4) + 2;

		vertexIndexes[(i * 6) + 3] = (i * 4) + 0;
		vertexIndexes[(i * 6) + 4] = (i * 4) + 2;
		vertexIndexes[(i * 6) + 5] = (i * 4) + 3;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(ParticleVertex), particleVertex, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxParticles * 6 * sizeof(unsigned int), vertexIndexes, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	// Texcoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete[] vertexIndexes;
}

ParticleEmitter::~ParticleEmitter() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete[] particles;
	delete[] particleVertex;
}

void ParticleEmitter::Render(BaseCamera& camera) {

	glm::vec3 right = camera.Right();
	glm::vec3 up = camera.Up();
	glm::vec3 forward =	camera.Forward();
	glm::mat3 billboard = glm::mat3(glm::vec3(right.x, right.y, right.z), glm::vec3(up.x, up.y, up.z), glm::vec3(forward.x, forward.y, forward.z));

	float size = 1.0f / 12.0f;
	float gravityStrength = 6.0f;
	for(int i = 0; i < totalParticles; i++) {
		Particle* particle = &particles[i];

		particle->velocity += gravity * Time::DeltaTime() * gravityStrength;

		CheckCollision(*particle);
		particle->position += particle->velocity * Time::DeltaTime();

		particleVertex[(i * 4) + 0].position = billboard * glm::vec3(-size, -size, 0) + particle->position;
		particleVertex[(i * 4) + 1].position = billboard * glm::vec3(-size, size, 0) + particle->position;
		particleVertex[(i * 4) + 2].position = billboard * glm::vec3(size, size, 0) + particle->position;
		particleVertex[(i * 4) + 3].position = billboard * glm::vec3(size, -size, 0) + particle->position;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, totalParticles * 4 * sizeof(ParticleVertex), particleVertex);

	shader->SetMatrix4("projectionView", camera.ProjectionView());
	shader->SetTextureUnit("terrainTexture", 4);

	BlockManager::GetTileMap(SpriteSheet::BLOCK)->BindTexture(4);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, totalParticles * 6, GL_UNSIGNED_INT, 0);
}
void ParticleEmitter::Update() {
	currentLife += Time::DeltaTime();
}
bool ParticleEmitter::IsFinished() const {
	return currentLife >= maxLife;
}
void ParticleEmitter::CheckCollision(Particle& particle) const {
	glm::vec3 pos = particle.position;
	glm::vec3 vel = particle.velocity * Time::DeltaTime();

	// X
	if(BlockManager::GetBlockData(World::GetChunkManager().GetBlockLock(pos.x + vel.x, pos.y, pos.z))->walkThrough) {
		particle.position.x = glm::floor(particle.position.x);
		particle.velocity.x = 0;
		pos.x = particle.position.x;
	}

	// Z
	if(BlockManager::GetBlockData(World::GetChunkManager().GetBlockLock(pos.x, pos.y, pos.z + vel.z))->walkThrough) {
		particle.position.z = glm::floor(particle.position.z);
		particle.velocity.z = 0;
		pos.z = particle.position.z;
	}

	// Y
	if(BlockManager::GetBlockData(World::GetChunkManager().GetBlockLock(pos.x, pos.y + vel.y, pos.z))->walkThrough) {
		particle.position.y = glm::floor(particle.position.y);
		particle.velocity.y = 0;
		pos.y = particle.position.y;
	}
}

