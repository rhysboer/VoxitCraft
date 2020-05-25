#version 330 core

// UP
const float light[4] = float[](1.0, 0.8, 0.6, 0.3);

uniform sampler2D texture1;

in vec2 _texCoords;
in vec3 _normals;
in float _ambient;

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
	vec4 texel = texture(texture1, _texCoords);
	if(texel.a < 0.5)
		discard;

	float ambient = (_ambient + 1.0) / 4.0; 
	FragColor = vec4(texel.rgb * GetLight(_normals) * ambient, texel.a);
}