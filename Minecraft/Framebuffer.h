#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Engine.h"

enum class FramebufferType {
	TEXTURE_RGB = 0x1907,
	TEXTURE_RGBA = 0x1908,
	DEPTH = 0x1902
};

class Framebuffer {
public:
	Framebuffer(const glm::vec2& size, const FramebufferType& type);
	~Framebuffer();

	void Render_Begin();
	void Render_End();

	void BindTexture(const int& i) const;
	unsigned int GetTexture() const;

private:
	glm::vec2 framebufferSize;
	glm::vec2 screenSize;

	unsigned int fbo;
	unsigned int rbo; // Renderbuffer
	unsigned int texture;
};

