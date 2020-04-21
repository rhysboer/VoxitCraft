#include "Framebuffer.h"

Framebuffer::Framebuffer(const glm::vec2& size, const FramebufferType& type) : framebufferSize(size), screenSize(0) {
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &texture);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, (int)type, size.x, size.y, 0, (int)type, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach Texture to Framebuffer
	int attachment = (type == FramebufferType::DEPTH) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

	if(type == FramebufferType::DEPTH) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else {
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		// Attach renderbuffer to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

	// Check if framebuffer is complete
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Framebuffer - failed to create framebuffer!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
	// DELETE BUFFER
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

void Framebuffer::BindTexture(const int& i) const {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int Framebuffer::GetTexture() const {
	return texture;
}
