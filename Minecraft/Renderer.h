#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "BaseCamera.h"
#include "ShaderManager.h"

struct Line {
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 colour;

	unsigned int vao;
	unsigned int vbo;
};

class Renderer {
public:

	static void AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(0.5f));
	static void CreateBox(glm::vec3 position, glm::vec3 color = glm::vec3(1,0,0));
	static void Render(BaseCamera& camera);
	static void Clear();

private:

	static Shader* shader;

	static std::vector<Line> lines;
};

