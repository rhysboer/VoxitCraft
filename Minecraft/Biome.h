#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include "BlockManager.h"
#include "Chunk.h"

struct BiomeGenParameters {
	unsigned int structureDensity = 150;
	unsigned int octaves = 5;
	float scale = 128.0f;
	float maxHeight = 48.0f;
	float yOffset = 0.0f;
};

class Biome {
public:

	virtual BlockIDs GetBlock(const int& height) const = 0;
	virtual void AddStructure(Chunk& chunk, const glm::vec3& position) const = 0;

	void GetHeightMap(std::array<float, Chunk::CHUNK_SLICE>& heightMap, const Chunk& chunk);
	float GetHeightAt(const int& world_x, const int& world_z);

	const BiomeGenParameters& GetGenerationParameters() const;

protected:

	std::string name = "Biome";
	BiomeGenParameters genParameters = BiomeGenParameters();

protected:

	glm::vec3 heightMapCoords = glm::vec3(std::numeric_limits<float>::max());
	std::array<float, Chunk::CHUNK_SLICE> heightMapCache = std::array<float, Chunk::CHUNK_SLICE>();
};

