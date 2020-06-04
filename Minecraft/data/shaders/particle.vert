#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec2 textureCoords; // Tex Coordinates

uniform mat4 projectionView;

out vec2 _texCoords;

void main()
{
	_texCoords = textureCoords;

	gl_Position = projectionView * vec4(vertex, 1);
}