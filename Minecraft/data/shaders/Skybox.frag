#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;


uniform samplerCube cubemap;

in vec3 _texCoords;

//out vec4 FragColor;

void main()
{
    FragColor = texture(cubemap, _texCoords);
    BloomColor = vec4(0, 0, 0, 1);
}