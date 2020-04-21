#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 textureCoords; // Tex Coordinates

uniform mat4 model;
uniform mat4 projectionView;

out vec2 _texCoords;
out vec3 _normals;

void main()
{
	_texCoords = textureCoords;
	_normals = normals;
	
	gl_Position = projectionView * model * vec4(vertex, 1);
}