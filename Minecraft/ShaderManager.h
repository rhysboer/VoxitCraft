#pragma once
#include <fstream>
#include <map>
#include "Shader.h"

#define PATH "./data/shaders/"

class ShaderManager {
public:
	enum class ShaderType {
		GRAPHIC, // Vertex & Fragment Shaders
		GRAPHIC_AND_GEOMETRY, // Vertex, Fragment and Geometry
		COMPUTE,  // Compute Shaders
	};

	static void InitShaders();
	static void DestroyShaders();
	
	static Shader* AddShader(const char* fileNames, ShaderType type = ShaderType::GRAPHIC);
	static Shader* GetShader(const char* name, ShaderType type = ShaderType::GRAPHIC);

private:

	static std::map<const char*, Shader*> graphicShaders;
	static std::map<const char*, Shader*> computeShaders;
};

