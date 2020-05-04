#version 330 core
layout (location = 0) in vec2 vertex; // Position
layout (location = 1) in vec2 texCoords; // TexCoords

out vec2 _texCoords;

void main()
{
	_texCoords = texCoords;
	gl_Position = vec4(vertex, 0, 1);
}