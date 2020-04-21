#version 330 core

// UP
const float light[4] = float[](1.0, 0.8, 0.6, 0.3);

uniform sampler2D texture1;
uniform float time;

in vec2 _texCoords;
in vec3 _normals;

out vec4 FragColor;


void main()
{
	float texIndex = mod(floor(time * 4), 16) / 16.0;

	vec2 coords = _texCoords;
	coords.x += texIndex;

	vec4 texel = texture(texture1, coords);
	FragColor = vec4(texel.rgb, 0.8); 
}