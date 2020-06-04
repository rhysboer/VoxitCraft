#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;


const float light[4] = float[](1.0, 0.8, 0.6, 0.3);
const float timeSpeed = 0.03;

uniform float timeOfDay;
uniform sampler2D terrainTexture;
uniform sampler2D bloomTexture;

in vec2 _texCoords;
in vec3 _normals;
in float _ambient;
in vec2 _light;

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
	// Bloom
	BloomColor = texture(bloomTexture, _texCoords);

	// Terrain
	vec4 sample = texture(terrainTexture, _texCoords);
	if(sample.a < 0.5)
		discard;

	float ambient = clamp((_ambient + 1.0) / 4.0, 0.50, 1.0); 
	
	float light = clamp(_light.x / 15.0, 0.0, 1.0);
	float sun = clamp((_light.y / 15.0) * ((sin(timeOfDay * timeSpeed) + 1) / 2.0), 0.2, 1.0);


	vec3 colour = (sample.rgb * GetFaceLight(_normals) * ambient) * max(light, sun);

	FragColor = vec4(colour, sample.a);
}