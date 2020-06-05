#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;
const float timeSpeed = 0.03;

uniform float timeOfDay;
uniform sampler2D terrainTexture;

in vec2 _texCoords;
in float _light;

void main()
{

	float light = clamp((_light / 15.0) * ((sin(timeOfDay * timeSpeed) + 1) / 2.0), 0.2, 1.0);
	vec4 sample = texture(terrainTexture, _texCoords).rgba;

	FragColor = vec4(sample.rgb * light, sample.a);

	BloomColor = vec4(0, 0, 0, sample.a);
}