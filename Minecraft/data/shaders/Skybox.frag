#version 330 core

uniform samplerCube cubemap;

in vec3 _texCoords;

out vec4 FragColor;

void main()
{
    FragColor = texture(cubemap, _texCoords);
}