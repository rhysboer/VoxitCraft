// LINE VERT
#version 330 core
layout (location = 0) in vec3 aPos; // VERTICE POSITION

uniform mat4 model;		// LOCAL
uniform mat4 projectionView;

void main()
{
    gl_Position =  projectionView * model * vec4(aPos, 1.0);
}