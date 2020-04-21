#include "Biome.h"

void Biome::GetHeightMap(std::array<float, Chunk::CHUNK_SLICE>& heightMap, const Chunk& chunk) {
	if(chunk.worldCoord == heightMapCoords) {
		for(int i = 0; i < heightMap.size(); i++)
			heightMap[i] = heightMapCache[i];

		return;
	}

	for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
		for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
			float sample = 0.0f;
			float totalAmplitude = 0.0f;
			for(int i = 0; i < genParameters.octaves; i++) {
				float pow = std::pow(2.0f, i);
				float amplitude = 1.0f / pow;

				sample += ((glm::simplex(glm::vec2(((x + chunk.worldCoord.x) / genParameters.scale) * pow, ((z + chunk.worldCoord.z) / genParameters.scale) * pow)) + 1.0f) / 2.0f) * amplitude;
				totalAmplitude += amplitude;
			}

			sample /= totalAmplitude;
			sample = (sample * genParameters.maxHeight) + genParameters.yOffset;

			heightMap[(z * Chunk::CHUNK_SIZE) + x] = sample;
			heightMapCache[(z * Chunk::CHUNK_SIZE) + x] = sample;
		}
	}

	heightMapCoords = chunk.worldCoord;
}

float Biome::GetHeightAt(const int& world_x, const int& world_z) {
	float sample = 0.0f;
	float totalAmplitude = 0.0f;
	for(int i = 0; i < genParameters.octaves; i++) {
		float pow = std::pow(2.0f, i);
		float amplitude = 1.0f / pow;

		sample += ((glm::simplex(glm::vec2(((world_x) / genParameters.scale) * pow, ((world_z) / genParameters.scale) * pow)) + 1.0f) / 2.0f) * amplitude;
		totalAmplitude += amplitude;
	}

	sample /= totalAmplitude;
	sample = (sample * genParameters.maxHeight) + genParameters.yOffset;
	return sample;
}

const BiomeGenParameters& Biome::GetGenerationParameters() const {
	return genParameters;
}
