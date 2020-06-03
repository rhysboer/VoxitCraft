#include "Framebuffer.h"

Framebuffer::Framebuffer(const glm::vec2& size, const FramebufferType& type, unsigned int colourAttachments)
	: framebufferSize(size), viewportSize(0), fbo(0), rbo(0), colourTexture(0), depthTexture(0), totalColourAttachments(colourAttachments)
{
	/*
		Use renderbuffer for depth if you dont need to sample it later in a shader, but otherwise use a texture for depth so we can use it for sampling
	*/
	this->type = type;

	CreateFramebuffer(colourAttachments);
}

Framebuffer::~Framebuffer() {
	// Delete Textures
	for(int i = 0; i < colourTexture.size(); i++) {
		glDeleteTextures(1, &colourTexture[i]);
	}
	glDeleteTextures(1, &depthTexture);

	// Delete Buffers
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
}

void Framebuffer::Render_Begin() {
	// Get current screen size
	this->viewportSize = Engine::GetWindowSize();

	glViewport(0, 0, framebufferSize.x, framebufferSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::Render_End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, this->viewportSize.x, this->viewportSize.y);
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::SetSize(const glm::vec2& size) {
	this->framebufferSize = size;

	for(int i = 0; i < colourTexture.size(); i++) {
		glDeleteTextures(1, &colourTexture[i]);
	}
	glDeleteTextures(1, &depthTexture);

	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);

	CreateFramebuffer(totalColourAttachments);
}

void Framebuffer::BindTextureColour(const unsigned int& texture, const unsigned int& index) const {
	if(!colourTexture.empty() && texture < colourTexture.size()) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, colourTexture[texture]);
	}
}

void Framebuffer::BindTextureDepth(const unsigned int& index) const {
	if(depthTexture != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
}

unsigned int Framebuffer::GetTexture(unsigned int index) const {
	if(index >= colourTexture.size()) return -1;
	return colourTexture[index];
}

void Framebuffer::CreateFramebuffer(const unsigned int& colourAttachments) {

	// Generate & bind Framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create Colour Texture
	if(type != FramebufferType::DEPTH_TEX) {

		unsigned int texture;
		std::vector<unsigned int> attachments = std::vector<unsigned int>();
		for(int i = 0; i < colourAttachments; i++) {
			// Generate Texture
			glGenTextures(1, &texture);

			glBindTexture(GL_TEXTURE_2D, texture);

			// Create Buffer Texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferSize.x, framebufferSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// Attach Texture to Framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
			attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
			
			colourTexture.emplace_back(texture);
		}

		glDrawBuffers(colourAttachments, attachments.data());

		// Create Depth Renderer
		if(type == FramebufferType::COLOUR_TEX_DEPTH) {
			// Generate Render Buffer (for depth & stencil)
			glGenRenderbuffers(1, &rbo);

			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize.x, framebufferSize.y);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	// Create Depth Texture
	if(type == FramebufferType::DEPTH_TEX || type == FramebufferType::COLOUR_TEX_DEPTH_TEX) {
		glGenTextures(1, &depthTexture);

		glBindTexture(GL_TEXTURE_2D, depthTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, framebufferSize.x, framebufferSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		if(type != FramebufferType::COLOUR_TEX_DEPTH_TEX) {
			// If framebuffer is depth only, we dont need to worry about drawing? 
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}

	// Check if framebuffer is complete
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Framebuffer - failed to create framebuffer!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
