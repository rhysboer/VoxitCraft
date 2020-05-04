#pragma once
#include <vector>
#include "BaseCamera.h"
#include "tiny_obj_loader.h"
#include "ShaderManager.h"
#include "Transform.h"

#define SHADER_MODEL "model"
#define SHADER_PROJECTIONVIEW "projectionView"

class Object3D {
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
	void Render(BaseCamera& camera);
	void RawRender(BaseCamera& camera);

	/* Setters */
	void SetShader(Shader* shader);
	
	/* Getters */
	Shader* GetShader() const;
	glm::vec3 GetPosition();
	Transform& GetTransform();

private:
	struct ObjectData {
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		unsigned int indicesCount;
	} object;

	Transform transform;
	Shader* shader;
};
