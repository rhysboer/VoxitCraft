#version 330 core

in vec2 _texCoords;
in vec3 _normals;

out vec4 FragColor;

void main()
{
	FragColor = vec4((_normals + 1.0) / 2, 1.0);
}