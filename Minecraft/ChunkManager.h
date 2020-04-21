#pragma once
#include <unordered_map>
#include <thread>
#include <array>
#include <mutex>
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "ShaderManager.h"
#include "WorldGeneration.h"
#include "Camera.h"
#include "Chunk.h"

#include "TileMap.h"

#include "Object3D.h"
#include "Engine.h"

class ChunkManager {
public:
	
	ChunkManager();
	~ChunkManager();

	void Update();
	void Render(Camera& camera);


	BlockIDs GetBlock(const glm::vec3& worldPosition) const;

	void SetBlock(const float& x, const float& y, const float& z, const BlockIDs& block);
	void SetBlock(const glm::vec3& worldPosition, const BlockIDs& block);

	friend Chunk;

private:

	WorldGeneration worldGen = WorldGeneration();

	// Multithreaded Funtions
	void ChunkLoader();


	Chunk* CreateChunk(const glm::ivec2& index);
	
	// Returns the chunk, otherwise returns nullptr;
	Chunk* FindChunk(const glm::ivec2& index) const;
	Chunk* FindChunk(const glm::vec3& worldPosition) const;
	Chunk* FindChunk(const float& x, const float& z) const;

	glm::ivec2 PositionToChunk(const glm::vec3& worldPosition) const;

private:
	std::unordered_map<glm::ivec2, Chunk*> chunks = std::unordered_map <glm::ivec2, Chunk*>();
	Chunk* cacheChunk = nullptr;

	TileMap* terrainTexture;
	TileMap* waterTexture;

	Shader* solidShader;
	Shader* waterShader;

	glm::vec3 position = glm::vec3(0);

	// TESTING
	std::mutex mutex;
	std::thread thread;
	bool ending = false;

	bool START = false;
};



