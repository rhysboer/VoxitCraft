#pragma once
#include "Biome.h"
class BiomeDesert : public Biome {
public:

	BiomeDesert();
	~BiomeDesert();

	// Inherited via Biome
	virtual BlockIDs GetBlock(const int& height) const override;
	//virtual void AddStructure(Chunk& chunk, const glm::vec3& position) const override;

private:
};

