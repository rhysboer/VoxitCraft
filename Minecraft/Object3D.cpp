#include "Object3D.h"

Object3D::Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<int>& vertex_data_attributes, const std::vector<unsigned int>& indices) 
	: transform(), render(RenderType::TRIANGLES) {
	this->transform.SetPosition(position);

	// Generate Vertex & Buffer Array
	glGenVertexArrays(1, &object.VAO);
	glGenBuffers(1, &object.VBO);
	if(indices.size() > 0) 
		glGenBuffers(1, &object.EBO);

	// Bind Vertex Array
	glBindVertexArray(object.VAO);

	// Bind Buffer Array
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO);
	// Bind Buffer Array Data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), &vertex_data[0], GL_STATIC_DRAW);

	// Bind Indices
	if(indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}

	int attributesPerVertex = 0;
	for(int i = 0; i < vertex_data_attributes.size(); i++)
		attributesPerVertex += vertex_data_attributes[i];

	unsigned int offset = 0;
	for(int i = 0; i < vertex_data_attributes.size(); i++) {
		glVertexAttribPointer(i, vertex_data_attributes[i], GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
	
		offset += vertex_data_attributes[i];
	}

	object.indicesCount = (indices.size() <= 0) ? (sizeof(float) * vertex_data.size()) / attributesPerVertex : indices.size();

	// Unbind Vertex Array
	glBindVertexArray(0);
	
	// Get Default Shader
	shader = ShaderManager::GetShader("default");
}

Object3D::~Object3D() {
	if(object.VAO != 0) {
		glDeleteVertexArrays(1, &object.VAO);
		glDeleteBuffers(1, &object.VBO);
	
		if(object.EBO != 0)
			glDeleteBuffers(1, &object.EBO);
	}
}

#pragma region Primitives

Object3D* Object3D::CreateObject_Plane(const glm::vec3& position, const unsigned int& size) {
	std::vector<float> vertices = std::vector<float>();
	std::vector<int> attributes = std::vector<int>({3, 3, 2});
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	// Generate Vertices
	for(int y = 0; y <= size; y++) {
		for(int x = 0; x <= size; x++) {
			// Vertex
			vertices.push_back((x / (float)size) * 2.0 - 1.0);	// X
			vertices.push_back(0);								// Y
			vertices.push_back((y / (float)size) * 2.0 - 1.0);	// Z

			// Normals
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			// Texture Coorindates
			vertices.push_back(x / (float)size);
			vertices.push_back(y / (float)size);
		}
	}

	// Generate Indices
	const int sizeOffset = size + 1;
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			indices.push_back(x + (y * sizeOffset));
			indices.push_back(x + (y * sizeOffset) + 1);
			indices.push_back(((y + 1) * sizeOffset) + x);

			indices.push_back(x + (y * sizeOffset) + 1);
			indices.push_back(((y + 1) * sizeOffset) + x + 1);
			indices.push_back(((y + 1) * sizeOffset) + x);
		}
	}

	return new Object3D(position, vertices, attributes, indices);
}

Object3D* Object3D::CreateObject_Cube(const glm::vec3& position) {
	std::vector<float> vertices = std::vector<float>({
		/* 1-3 = Vertex Pos, 4-6 = Normals, 7-8 = Texture Coords*/
		/* FRONT */
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
		 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 2
						   
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 3
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 2
		 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1

		/* RIGHT */
		 1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
						   
		 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 /* BACK */
		-1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f,
						   
		 1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f,

		/* LEFT */
		-1.0f,-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
						  
		-1.0f, 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		/* TOP */
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						  
		 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		 /* BOT */
		-1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f,
						  
		 1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
	});

	// Attributes: Vertex Position, Normals Texture Coordinates
	std::vector<int> attributes = std::vector<int>({
		3, 3, 2
	});

	return new Object3D(position, vertices, attributes);
}

Object3D* Object3D::CreateObject_Quad(const glm::vec3& position) {
	std::vector<float> vertices = std::vector<float>({
		/* 1-3 = Vertex Pos, 4-6 = Normals, 7-8 = Texture Coords*/
		-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
						   		 
		 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
													 });

	// Attributes: Vertex Position, Texture Coordinates
	std::vector<int> attributes = std::vector<int>({
		3, 3, 2
	});

	return new Object3D(position, vertices, attributes);
}

Object3D* Object3D::CreateObject_FromFile(const glm::vec3& position, const char* path) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;

	std::string error;
	std::string warning;

	if(!tinyobj::LoadObj(&attrib, &shapes, &material, &warning, &error, path)) {
		printf("ERROR: Object3D - Failed to load object at (%s)\n", path);
		return nullptr;
	}

	if(!error.empty()) {
		printf("ERROR: Object3D - %s\n", error.c_str());
		return nullptr;
	}

	if(!warning.empty())
		printf("WARNING: Object3D - %s\n", warning.c_str());

	std::vector<float> vertex_data = std::vector<float>();
	std::vector<int> vertex_attributes = std::vector<int>({ 3 });

	if(attrib.normals.size() > 0)
		vertex_attributes.push_back(3);
	if(attrib.texcoords.size() > 0)
		vertex_attributes.push_back(2);

	for(size_t s = 0; s < 1; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for(size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				// Vertex Position
				vertex_data.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				vertex_data.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
				vertex_data.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

				// Vertex Normals
				if(attrib.normals.size() > 0) {
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 0]);
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 1]);
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}

				// Vertex Texcoords
				if(attrib.texcoords.size() > 0) {
					vertex_data.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
					vertex_data.push_back(1.0 - attrib.texcoords[2 * idx.texcoord_index + 1]);
				}
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return new Object3D(position, vertex_data, vertex_attributes);
}


#pragma endregion


void Object3D::Render(const glm::mat4& projectionView) {
	if(!shader) return;

	glBindVertexArray(object.VAO);

	shader->SetMatrix4(SHADER_MODEL, transform.GetMatrix());
	shader->SetMatrix4(SHADER_PROJECTIONVIEW, projectionView);

	if(render == RenderType::TRIANGLES) {
		if(object.EBO <= 0)
			glDrawArrays(GL_TRIANGLES, 0, object.indicesCount);
		else
			glDrawElements(GL_TRIANGLES, object.indicesCount, GL_UNSIGNED_INT, 0);
	} else if(render == RenderType::LINES) {
		glDrawArrays(GL_LINES, 0, object.indicesCount);
	}
}

void Object3D::Render(BaseCamera& camera) {
	Render(camera.ProjectionView());
}

void Object3D::RawRender(BaseCamera& camera) {
	if(!shader) return;

	glBindVertexArray(object.VAO);

	if(object.EBO <= 0)
		glDrawArrays(GL_TRIANGLES, 0, object.indicesCount);
	else
		glDrawElements(GL_TRIANGLES, object.indicesCount, GL_UNSIGNED_INT, 0);
}

void Object3D::BindVAO() const {
	glBindVertexArray(object.VAO);
}

void Object3D::BindVBO() const {
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO);
}

void Object3D::SetShader(Shader* shader) {
	this->shader = shader;
}

void Object3D::SetRender(const RenderType& render) {
	this->render = render;
}

Shader* Object3D::GetShader() const {
	return shader;
}

glm::vec3 Object3D::GetPosition() {
	return transform.GetPosition();
}

Transform& Object3D::GetTransform() {
	return transform;
}
