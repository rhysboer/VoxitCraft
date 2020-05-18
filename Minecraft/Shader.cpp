#include "Shader.h"

Shader::Shader() {
	this->shaderProgram = -1;
}

Shader::~Shader() {
	if(shaderProgram != 0)
		glDeleteProgram(shaderProgram);
}

bool Shader::LoadShader(std::string vertSource, std::string fragSource, std::string geomShader) {
	unsigned int shaderVertex;
	unsigned int shaderFragment;
	unsigned int shaderGeometry;

	shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	if(geomShader.length() > 0)
		shaderGeometry = glCreateShader(GL_GEOMETRY_SHADER);

	// VERTEX
	glShaderSource(shaderVertex, 1, StringHelper(vertSource), NULL);
	glCompileShader(shaderVertex);

	if(!ErrorHandler(shaderVertex, GL_COMPILE_STATUS)) return false;

	// FRAGMENT
	glShaderSource(shaderFragment, 1, StringHelper(fragSource), NULL);
	glCompileShader(shaderFragment);

	if(!ErrorHandler(shaderFragment, GL_COMPILE_STATUS)) return false;

	// GEOMETRY
	if(geomShader.length() > 0) {
		glShaderSource(shaderGeometry, 1, StringHelper(geomShader), NULL);
		glCompileShader(shaderGeometry);

		if(!ErrorHandler(shaderGeometry, GL_COMPILE_STATUS)) return false;
	}

	// LINK SHADER
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderVertex);
	glAttachShader(shaderProgram, shaderFragment);
	if(geomShader.length() > 0)
		glAttachShader(shaderProgram, shaderGeometry);

	glLinkProgram(shaderProgram);

	if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) return false;

	// Delete
	glDeleteShader(shaderVertex);
	glDeleteShader(shaderFragment);
	if(geomShader.length() > 0)
		glDeleteShader(shaderGeometry);

	return true;
}

bool Shader::LoadShader(const std::string& computeSource) {
	unsigned int shaderIndex;

	shaderIndex = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderIndex, 1, StringHelper(computeSource), NULL);
	glCompileShader(shaderIndex);

	if(!ErrorHandler(shaderIndex, GL_COMPILE_STATUS)) return false;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderIndex);
	glLinkProgram(shaderProgram);

	if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) return false;

	glDeleteShader(shaderIndex);

	return true;
}

void Shader::UseProgram() {
	glUseProgram(shaderProgram);
}

void Shader::SetFloat(const char* name, float value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding float Uniform: %s\n", name);
		return;
	}
	glUniform1f(loc, value);
}

void Shader::SetFloatArray(const char* name, const unsigned int& size, const float& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding float Uniform: %s\n", name);
		return;
	}

	glUniform1fv(loc, size, &value);
}

void Shader::SetInt(const char* name, int value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding int Uniform: %s\n", name);
		return;
	}
	glUniform1i(loc, value);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Mat4 Uniform: %s\n", name);
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMatrix3(const char* name, const glm::mat3& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Mat4 Uniform: %s\n", name);
		return;
	}
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVector2(const char* name, const glm::vec2& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Vec4 Uniform: %s\n", name);
		return;
	}

	glUniform2f(loc, value.r, value.g);
}

void Shader::SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Vec3 Uniform: %s\n", name);
		return;
	}

	glUniform2fv(loc, size, &value[0]);
}

void Shader::SetVector4(const char* name, const glm::vec4& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Vec4 Uniform: %s\n", name);
		return;
	}

	glUniform4f(loc, value.r, value.g, value.b, value.a);
}

void Shader::SetVector3(const char* name, const glm::vec3& value) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Vec3 Uniform: %s\n", name);
		return;
	}

	glUniform3f(loc, value.r, value.g, value.b);
}

void Shader::SetTextureUnit(const char* name, const unsigned int& textureUnit) {
	UseProgram();

	unsigned int loc = glGetUniformLocation(shaderProgram, name);
	if(loc == -1) {
		printf("Error finding Mat4 Uniform: %s\n", name);
		return;
	}
	glUniform1i(loc, textureUnit);
}

bool Shader::ErrorHandler(unsigned int toTest, unsigned int statusType) {
	int success;
	int logSize = 0;

	glGetShaderiv(toTest, GL_INFO_LOG_LENGTH, &logSize);
	char* log = new char[logSize];

	switch(statusType) {
		case GL_COMPILE_STATUS: // SHADER
			glGetShaderiv(toTest, GL_COMPILE_STATUS, &success);

			if(!success) {
				glGetShaderInfoLog(toTest, logSize, NULL, log);
				std::cout << "ERROR COMPILING SHADER FAILED!\n" << log << std::endl;
				return false;
			}

			break;
		case GL_LINK_STATUS: // LINK
			glGetProgramiv(toTest, GL_LINK_STATUS, &success);

			if(!success) {
				glGetProgramInfoLog(toTest, logSize, NULL, log);
				std::cout << "ERROR LINKING FAILED\n" << log << std::endl;
				return false;
			}

			break;
		default:
			printf("No Shader Error Status: %i\n", statusType);
			return false;
	}

	delete[] log;
	return true;
}
