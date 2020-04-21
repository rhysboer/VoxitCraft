#version 330 core
layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 view;

out vec3 _texCoords;

void main()
{
    _texCoords = vec3(vertex.x, vertex.y, vertex.z * -1);
    gl_Position = projection * view * vec4(vertex, 1.0);
}