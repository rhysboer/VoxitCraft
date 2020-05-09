// LINE VERT
#version 330 core
layout (location = 0) in vec3 aPos; // VERTICE POSITION

uniform mat4 _model;		// LOCAL
uniform mat4 _projectionView;
uniform vec3 _colour;


out vec3 outColour;

void main()
{
    gl_Position =  _projectionView * _model * vec4(aPos, 1.0);
	outColour = _colour;
}