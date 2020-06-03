#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

const float light[4] = float[](1.0, 0.8, 0.6, 0.3);

uniform sampler2D texture1;
uniform float time;

in vec2 _texCoords;
in vec3 _normals;

void main()
{
	float texIndex = mod(floor(time * 4), 16) / 16.0;

	vec2 coords = _texCoords;
	coords.x += texIndex;

	vec4 texel = texture(texture1, coords);

	FragColor = vec4(texel.rgb, 0.7); 

	BloomColor = vec4(0,0,0,1);
}