#include "ShaderManager.h"

std::map<const char*, Shader*> ShaderManager::graphicShaders = std::map<const char*, Shader*>();
std::map<const char*, Shader*> ShaderManager::computeShaders = std::map<const char*, Shader*>();

void ShaderManager::InitShaders() {
	AddShader("default");
}

Shader* ShaderManager::GetShader(const char* name, ShaderType type) {
	if(type == ShaderType::GRAPHIC || type == ShaderType::GRAPHIC_AND_GEOMETRY) {
		std::map<const char*, Shader*>::iterator iter = graphicShaders.find(name);
		if(iter != graphicShaders.end())
			return (*iter).second;
	} else if(type == ShaderType::COMPUTE) {
		std::map<const char*, Shader*>::iterator iter = computeShaders.find(name);
		if(iter != computeShaders.end())
			return (*iter).second;
	} else {
		printf("GetShader() : Failed to find shader type: %i\n", type);
		return nullptr;
	}

	printf("GetShader() : Failed to find shader: %s!\n", name);
	return nullptr;
}

Shader* ShaderManager::AddShader(const char* fileNames, ShaderType type) {
	std::string path = PATH;
	std::ifstream file;

	if(type == ShaderType::GRAPHIC || type == ShaderType::GRAPHIC_AND_GEOMETRY) {
		std::string vertSource;
		std::string fragSource;
		std::string geomSource = "";

		// Vertex
		file.open(path + fileNames + ".vert");

		if(!file.is_open()) {
			printf("Failed to load Vertex Shader: %s\n", fileNames);
			file.close();
			return nullptr;
		}

		vertSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Fragment
		file.open(path + fileNames + ".frag");

		if(!file.is_open()) {
			printf("Failed to load Fragment Shader: %s\n", fileNames);
			file.close();
			return nullptr;
		}

		fragSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// GEOMETRY
		if(type == ShaderType::GRAPHIC_AND_GEOMETRY) {
			file.open(path + fileNames + ".geom");

			if(!file.is_open()) {
				printf("Failed to load Geometry Shader: %s\n", fileNames);
				file.close();
				return nullptr;
			}

			geomSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
		}

		Shader* shader = new Shader();
		if(shader->LoadShader(vertSource, fragSource, geomSource)) {
			graphicShaders.insert(std::pair<const char*, Shader*>(fileNames, shader));
			return shader;
		}
	} else {
		file.open(path + fileNames + ".comp");

		if(!file.is_open()) {
			printf("Failed to load Compute Shader: %s\n", fileNames);
			file.close();
			return nullptr;
		}

		std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		Shader* shader = new Shader();
		if(shader->LoadShader(source)) {
			computeShaders.insert(std::pair<const char*, Shader*>(fileNames, shader));
			return shader;
		}
	}

	return nullptr;
}
