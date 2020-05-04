#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Engine.h"

enum class FramebufferType {
	COLOUR, // Colour texture with renderbuffer depth
	DEPTH, // Depth texture
	COLOUR_AND_DEPTH, // Colour and depth texture
};

class Framebuffer {
public:
	Framebuffer(const glm::vec2& size, const FramebufferType& type);
	~Framebuffer();

	void Render_Begin();
	void Render_End();

	// If texture equals COLOUR_AND_DEPTH, colour will be bound to INDEX and depth will be bount to INDEX + 1
	void BindTextures(const unsigned int& index) const;
	void BindTextureColour(const unsigned int& index) const;
	void BindTextureDepth(const unsigned int& index) const;

	unsigned int GetTexture() const;

private:
	glm::vec2 framebufferSize;
	glm::vec2 screenSize;

	FramebufferType type;

	unsigned int fbo; // Framebuffer
	unsigned int rbo; // Renderbuffer

	unsigned int colourTexture;
	unsigned int depthTexture;
};

