#version 330 core

in vec2 _texCoords;

out vec4 FragColor;

uniform sampler2D sampleTexture;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(sampleTexture, 0);
	vec3 result = texture(sampleTexture, _texCoords).rgb * weight[0];

    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampleTexture, _texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(sampleTexture, _texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampleTexture, _texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(sampleTexture, _texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

	FragColor = vec4(result, 1.0);
}