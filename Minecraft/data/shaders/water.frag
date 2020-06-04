#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

const float light[4] = float[](1.0, 0.8, 0.6, 0.3);
const float timeSpeed = 0.03;

uniform float timeOfDay;
uniform sampler2D texture1;
uniform float time;

in vec2 _texCoords;
in vec3 _normals;
in vec2 _light;

void main()
{
	float texIndex = mod(floor(time * 4), 16) / 16.0;

	vec2 coords = _texCoords;
	coords.x += texIndex;

	vec4 texel = texture(texture1, coords);
	
	float light = clamp(_light.x / 15.0, 0.0, 1.0);
	float sun = clamp((_light.y / 15.0) * ((sin(timeOfDay * timeSpeed) + 1) / 2.0), 0.2, 1.0);

	FragColor = vec4(texel.rgb * max(light, sun), 0.7); 

	BloomColor = vec4(0,0,0,0);
}