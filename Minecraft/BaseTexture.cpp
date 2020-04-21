#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include "BaseTexture.h"

BaseTexture::BaseTexture() : texture(0) {}

BaseTexture::~BaseTexture() {
	if(texture != 0)
		glDeleteTextures(1, &texture);
}

void BaseTexture::BindTexture(const unsigned int& index) const {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int BaseTexture::GetTexture() const {
	return texture;
}

TextureData* BaseTexture::LoadFile(const char* path, const bool& flipY) const {
	TextureData* textureData = new TextureData();

	stbi_set_flip_vertically_on_load(flipY);
	textureData->data = stbi_load(path, &textureData->width, &textureData->height, &textureData->format, 0);

	if(textureData->data == nullptr) {
		delete textureData;
		return nullptr;
	}

	return textureData;
}

void BaseTexture::GenerateTexture(const TextureData& data) {
	int format = data.GetFormat();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, data.width, data.height, 0, format, GL_UNSIGNED_BYTE, data.data);
}

void BaseTexture::DestroyTextureData(TextureData* data) {
	if(data)
		stbi_image_free(data->data);
}
