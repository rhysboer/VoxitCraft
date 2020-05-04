#include "Framebuffer.h"

Framebuffer::Framebuffer(const glm::vec2& size, const FramebufferType& type) : framebufferSize(size), screenSize(0), fbo(0), rbo(0), colourTexture(0), depthTexture(0) {
	/*
		Use renderbuffer for depth if you dont need to sample it later in a shader, but otherwise use a texture for depth so we can use it for sampling
	*/
	this->type = type;

	// Generate & bind Framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create Colour Texture
	if(type == FramebufferType::COLOUR || type == FramebufferType::COLOUR_AND_DEPTH) {
		// Generate Texture
		glGenTextures(1, &colourTexture);

		glBindTexture(GL_TEXTURE_2D, colourTexture);

		// Create Buffer Texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind Texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Attach Texture to Framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);


		if(type != FramebufferType::COLOUR_AND_DEPTH) {
			// Generate Render Buffer (for depth & stencil)
			glGenRenderbuffers(1, &rbo);

			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	if(type == FramebufferType::DEPTH || type == FramebufferType::COLOUR_AND_DEPTH) {
		glGenTextures(1, &depthTexture);

		glBindTexture(GL_TEXTURE_2D, depthTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		if(type != FramebufferType::COLOUR_AND_DEPTH) {
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

Framebuffer::~Framebuffer() {
	// Delete Textures
	if(colourTexture != 0)
		glDeleteTextures(1, &colourTexture);
	if(depthTexture != 0)
		glDeleteTextures(1, &depthTexture);

	// Delete Buffer
	glDeleteFramebuffers(1, &fbo);

	if(rbo != 0)
		glDeleteRenderbuffers(1, &rbo);
}

void Framebuffer::Render_Begin() {
	// Get current screen size
	this->screenSize = Engine::GetWindowSize();

	glViewport(0, 0, framebufferSize.x, framebufferSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::Render_End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, this->screenSize.x, this->screenSize.y);
}

void Framebuffer::BindTextures(const unsigned int& index) const {
	if(type == FramebufferType::COLOUR) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, colourTexture);
	} else if(type == FramebufferType::DEPTH) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	} else if(type == FramebufferType::COLOUR_AND_DEPTH) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, colourTexture);

		glActiveTexture(GL_TEXTURE0 + index + 1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
}

void Framebuffer::BindTextureColour(const unsigned int& index) const {
	if(colourTexture != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, colourTexture);
	}
}

void Framebuffer::BindTextureDepth(const unsigned int& index) const {
	if(depthTexture != 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
}

unsigned int Framebuffer::GetTexture() const {
	return colourTexture;
}
