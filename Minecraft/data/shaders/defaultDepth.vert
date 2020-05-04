// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 vertex; // VERTICE POSITION

uniform vec3 position;
uniform mat4 projectionView;

void main()
{
	mat4 model = mat4(1.0f);
	model[3] = vec4(position, 1.0f);
	
	gl_Position =  projectionView * model * vec4(vertex, 1.0f);
}