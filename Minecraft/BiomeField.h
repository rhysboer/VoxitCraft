#pragma once
#include "Biome.h"
class BiomeField : public Biome {
public:

	BiomeField();
	~BiomeField();

	// Inherited via Biome
	virtual BlockIDs GetBlock(const int& height) const override;
	//virtual void AddStructure(Chunk& chunk, const glm::vec3& position) const override;
	virtual void AddStructure(const glm::vec3& position, std::array<BlockIDs, Chunk::CHUNK_MASS>& chunkBlocks, std::vector<glm::vec3>& outPos, std::vector<BlockIDs>& outBlocks) override;

private:

	const int TREE_STRUCTURE[6][7][7] =
	{
		{
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 0, 1, 1, 1 },
			{ 0, 1, 1, 1, 1, 1, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 0, 1, 1, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
		},
		{
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 0 },
			{ 1, 1, 1, 0, 1, 1, 1 },
			{ 0, 1, 1, 1, 1, 1, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0 },
		}
	};
};

