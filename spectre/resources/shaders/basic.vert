#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 textureCoordinate;

out vec4 vColor;
out vec2 vTextureCoordinate;

uniform mat4 uProjection;

void main()
{
    vColor = color;
    vTextureCoordinate = textureCoordinate;
    gl_Position = uProjection * vec4(position, 1.0f);
}
