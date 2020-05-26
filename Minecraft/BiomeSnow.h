#pragma once
#include "Biome.h"
class BiomeSnow : public Biome {
public:
	BiomeSnow();
	~BiomeSnow();

	// Inherited via Biome
	virtual BlockIDs GetBlock(const int& height) const override;
	//virtual void AddStructure(Chunk& chunk, const glm::vec3& position) const override;
};

