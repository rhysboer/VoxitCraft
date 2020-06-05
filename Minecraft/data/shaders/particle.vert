#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec2 textureCoords; // Tex Coordinates
layout (location = 2) in float lightLevel;

uniform mat4 projectionView;

out vec2 _texCoords;
out float _light;

void main()
{
	_texCoords = textureCoords;
	_light = lightLevel;

	gl_Position = projectionView * vec4(vertex, 1);
}