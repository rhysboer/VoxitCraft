#pragma once
#include <vector>
#include "Camera.h"
#include "tiny_obj_loader.h"
#include "ShaderManager.h"
#include "Transform.h"

#define SHADER_MODEL "model"
#define SHADER_PROJECTIONVIEW "projectionView"

class Object3D : public Transform {
public:
	Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<int>& vertex_data_attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
	~Object3D();

#pragma region Object Creation

	static Object3D* CreateObject_Plane(const glm::vec3& position, const unsigned int& size);
	static Object3D* CreateObject_Cube(const glm::vec3& position);
	static Object3D* CreateObject_Quad(const glm::vec3& position);
	static Object3D* CreateObject_FromFile(const glm::vec3& position, const char* path);

#pragma endregion


	void Render(const glm::mat4& projectionView);
	void Render(Camera& camera);
	void RawRender(Camera& camera);

	/* Setters */
	void SetShader(Shader* shader);
	
	/* Getters */
	Shader* GetShader() const;

private:
	struct ObjectData {
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		unsigned int indicesCount;
	} object;

	Shader* shader;
};
