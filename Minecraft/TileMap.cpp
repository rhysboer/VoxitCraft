#include "TileMap.h"

TileMap::TileMap(const char* path, const unsigned int& rows, const unsigned int& columns) {
	texture = new Sprite(path);

	this->rows = rows;
	this->columns = columns;
	this->sizeX = 1.0f / rows;
	this->sizeY = 1.0f / columns;
}

TileMap::~TileMap() {
	delete texture;
}

void TileMap::BindTexture(const unsigned int& index) const {
	texture->BindTexture(index);
}

void TileMap::GetSpriteCoordinates(const unsigned int& index, std::array<glm::vec2, 4>& coords) const {
	float x = (index % rows) / (float)rows;
	float y = 1.0f - ((index / columns) * sizeY);

	coords[0].x = x + sizeX;
	coords[0].y = y - sizeY;

	coords[1].x = x;
	coords[1].y = y - sizeY;

	coords[2].x = x;
	coords[2].y = y;

	coords[3].x = x + sizeX;
	coords[3].y = y;
}
