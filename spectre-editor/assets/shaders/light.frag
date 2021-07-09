#version 450 core

layout (location = 0) out vec4 color;

uniform vec4 u_ObjectColor;

void main()
{
    color = u_ObjectColor;
}
