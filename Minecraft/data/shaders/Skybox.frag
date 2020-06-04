#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

const vec3 dayTopColour = vec3(11,90,182) / 255;
const vec3 dayBotColour = vec3(57,182,214) / 255;
const vec3 nightBotColour = vec3(17,85,155) / 255;
const vec3 nightTopColour = vec3(19,21,22) / 255;

const float timeSpeed = 0.03;

uniform float timeOfDay;
uniform samplerCube cubemap;

in vec3 _texCoords;
in vec3 _vertexPos;

void main()
{
    float point = (normalize(_vertexPos).y + 1.0) / 2.0;

    float dayTime = ((sin(timeOfDay * timeSpeed) + 1.0) / 2.0);

    vec3 colour = mix(mix(nightBotColour, nightTopColour, point), mix(dayBotColour, dayTopColour, point), dayTime);


    FragColor =  vec4(colour, 1.0); 
    BloomColor = vec4(0, 0, 0, 1);
}