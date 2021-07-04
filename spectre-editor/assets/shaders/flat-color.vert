#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vColor = color;
    gl_Position = uProjection * uView * uModel * position;
}
