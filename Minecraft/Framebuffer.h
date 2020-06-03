#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Engine.h"

enum class FramebufferType {
	DEPTH_TEX, // Depth texture
	COLOUR_TEX, // Colour texture with NO renderbuffer
	COLOUR_TEX_DEPTH, // Colour texture with renderbuffer depth
	COLOUR_TEX_DEPTH_TEX, // Colour and depth texture
};

class Framebuffer {
public:
	Framebuffer(const glm::vec2& size, const FramebufferType& type, unsigned int colourAttachments = 1);
	~Framebuffer();

	void Render_Begin();
	void Render_End();
	void Bind();

	void SetSize(const glm::vec2& size);

	void BindTextureColour(const unsigned int& texture, const unsigned int& index) const;
	void BindTextureDepth(const unsigned int& index) const;

	unsigned int GetTexture(unsigned int index) const;

private:

	void CreateFramebuffer(const unsigned int& colourAttachments = 1);

	glm::vec2 framebufferSize;
	glm::vec2 viewportSize; // Viewport size before resizing

	FramebufferType type;

	unsigned int fbo; // Framebuffer
	unsigned int rbo; // Renderbuffer

	unsigned int totalColourAttachments;
	std::vector<unsigned int> colourTexture = std::vector<unsigned int>();
	unsigned int depthTexture;
};

