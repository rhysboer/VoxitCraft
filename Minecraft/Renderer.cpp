#include "Renderer.h"

std::vector<Line> Renderer::lines = std::vector<Line>();
Shader* Renderer::shader;

void Renderer::AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
	Line line = Line();
	line.start = start;
	line.end = end;
	line.colour = color;

	glm::vec3 vec[2] = {
		start,end
	};

	glGenVertexArrays(1, &line.vao);
	glGenBuffers(1, &line.vbo);

	glBindVertexArray(line.vao);

	glBindBuffer(GL_ARRAY_BUFFER, line.vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &vec[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lines.push_back(line);
}

void Renderer::CreateBox(glm::vec3 p, glm::vec3 color) {
	float offset = 0.5f;

	// Top Back Left to Top Back Right
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z - offset)), glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z - offset)), glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z + offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z + offset)), glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z + offset)), color);
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z + offset)), glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z - offset)), glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z - offset)), glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z + offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z + offset)), glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z + offset)), color);
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z + offset)), glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z - offset)), glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z - offset)), glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z - offset)), color);
	AddLine(glm::floor(glm::vec3(p.x + offset, p.y + offset, p.z + offset)), glm::floor(glm::vec3(p.x + offset, p.y - offset, p.z + offset)), color);
	AddLine(glm::floor(glm::vec3(p.x - offset, p.y + offset, p.z + offset)), glm::floor(glm::vec3(p.x - offset, p.y - offset, p.z + offset)), color);
}

void Renderer::Render(BaseCamera& camera) {
	if(shader == nullptr)
		shader = ShaderManager::AddShader("lineColour");

	shader->SetMatrix4("_projectionView", camera.ProjectionView());
	shader->SetMatrix4("_model", glm::mat4(1.0f));

	for(int i = 0; i < lines.size(); i++) {
		shader->SetVector3("_colour", lines[i].colour);
		glBindVertexArray(lines[i].vao);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

void Renderer::Clear() {
	for(int i = 0; i < lines.size(); i++){
		glDeleteBuffers(1, &lines[i].vbo);
		glDeleteVertexArrays(1, &lines[i].vao);
	}

	lines.clear();
}
