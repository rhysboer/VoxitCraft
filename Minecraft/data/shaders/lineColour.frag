// LINE FRAG
#version 330 core
out vec4 FragColor;

in vec3 outColour;

void main()
{
    FragColor = vec4(outColour, 1.0f);
} 