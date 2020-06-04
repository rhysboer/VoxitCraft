#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform sampler2D terrainTexture;

in vec2 _texCoords;

void main()
{

	vec4 sample = texture(terrainTexture, _texCoords).rgba;
	FragColor = sample;

	BloomColor = vec4(0, 0, 0, sample.a);
}