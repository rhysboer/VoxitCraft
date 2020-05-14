#version 330 core

const float spriteSize = 0.125;
const int spriteRow = 8;

const float light[4] = float[](1.0, 0.8, 0.6, 0.3);

uniform sampler2D texture1;
uniform float faceIndex[6];

in vec2 _texCoords;
in vec3 _normals;
in float _index;

out vec4 FragColor;

float GetLight(vec3 normal){
	if(normal.x != 0.0)
		return 0.8;
	if(normal.y > 0.0)
		return 1.0;
	if(normal.y < 0.0)
		return 0.4;
	if(normal.z != 0.0)
		return 0.6;
}

void main()
{
	vec2 coords = _texCoords;
	coords.x *= spriteSize;
	coords.y *= -spriteSize;

	// Offset
	float face = faceIndex[int(_index)];
	float xFace = mod(face, spriteRow);
	float yFace = floor(face / spriteRow);

	coords.x += spriteSize * xFace;
	coords.y += -spriteSize * yFace;

	vec4 sample = texture(texture1, coords);

	if(sample.a < 0.5)
		discard;

	FragColor = vec4(sample.rgb * GetLight(_normals), 1.0);
}