#include "PostProcess.h"

PostProcess::PostProcess(const char* shaderPath, const float& sizeX, const float& sizeY, const FramebufferType& type) {
	framebuffer = new Framebuffer(glm::vec2(sizeX, sizeY), type);

	shader = ShaderManager::AddShader("postProcess");

	const float verticies[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), &verticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

PostProcess::~PostProcess() {
	delete framebuffer;
}

Shader& PostProcess::GetShader() const {
	return *shader;
}

Framebuffer& PostProcess::GetBuffer() const {
	return *framebuffer;
}

void PostProcess::Begin() {
	framebuffer->Render_Begin();
}

void PostProcess::End() {
	framebuffer->Render_End();
}

void PostProcess::Render() {
	// Render screen quad
	shader->UseProgram();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
