#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "StringHelper.h"

class ShaderManager;

class Shader {
public:
	Shader();
	~Shader();

	void UseProgram();

	void SetFloat(const char* name, float value);
	void SetFloatArray(const char* name, const unsigned int& size, const float& value);
	void SetInt(const char* name, int value);
	void SetMatrix4(const char* name, const glm::mat4& value);
	void SetMatrix3(const char* name, const glm::mat3& value);
	void SetVector2(const char* name, const glm::vec2& value);
	void SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value);
	void SetVector3(const char* name, const glm::vec3& value);
	void SetVector4(const char* name, const glm::vec4& value);

	void SetTextureUnit(const char* name, const unsigned int& textureUnit);

protected:
	friend ShaderManager;

	bool LoadShader(std::string vertSource, std::string fragSource, std::string geomShader = ""); // Graphics Shader
	bool LoadShader(const std::string& computeSource); // Compute Shader

private:

	bool ErrorHandler(unsigned int toTest, unsigned int statusType);

	int shaderProgram;
};

