#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 textureCoords; // Tex Coordinates
layout (location = 3) in float ambient; // Ambient Occlusion
layout (location = 4) in vec2 light;

uniform vec3 position;
uniform mat4 projectionView;

out vec2 _texCoords;
out vec3 _normals;
out float _ambient;
out vec2 _light;

void main()
{
	_texCoords = textureCoords;
	_normals = normals;
	_ambient = ambient;
	_light = light;
	
	mat4 model = mat4(1);
	model[3] = vec4(position, 1);
	
	gl_Position = projectionView * model * vec4(vertex, 1);
}