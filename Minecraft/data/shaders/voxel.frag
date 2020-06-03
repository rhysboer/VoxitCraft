#version 330 core

// UP
const float light[4] = float[](1.0, 0.8, 0.6, 0.3);

uniform sampler2D texture1;

in vec2 _texCoords;
in vec3 _normals;
in float _ambient;
in vec2 _light;

out vec4 FragColor;

float GetFaceLight(vec3 normal){
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
	vec4 sample = texture(texture1, _texCoords);
	if(sample.a < 0.5)
		discard;

	float ambient = clamp((_ambient + 1.0) / 4.0, 0.50, 1.0); 
	
	float light = clamp(_light.x / 15.0, 0.0, 1.0);
	float sun = clamp(_light.y / 15.0, 0.2, 1.0);

	vec3 colour = (sample.rgb * GetFaceLight(_normals) * ambient) * max(light, sun);

	FragColor = vec4(colour, sample.a);
}