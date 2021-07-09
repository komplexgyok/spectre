#version 450 core

layout (location = 0) out vec4 color;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

void main()
{
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * u_LightColor;

    vec4 result = ambient * u_ObjectColor;

    color = result;
}
