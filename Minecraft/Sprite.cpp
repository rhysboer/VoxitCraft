#include "Sprite.h"

Sprite::Sprite() { }

Sprite::Sprite(const char* path) {
	LoadTexture(path);
}

Sprite::~Sprite() {
}

bool Sprite::LoadTexture(const char* path) {
	TextureData* texture = LoadFile(path);

	if(texture == nullptr) {
		printf("Sprite: ERROR - Couldn't find texture at (%s)\n", path);
		return false;
	}

	GenerateTexture(*texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	DestroyTextureData(texture);
	delete texture;

	return true;
}
