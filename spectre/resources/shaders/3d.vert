#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in float textureId;

out vec2 vTextureCoordinate;
out float vTextureId;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTextureCoordinate = textureCoordinate;
    vTextureId = textureId;
    gl_Position = uProjection * uView * uModel * vec4(position, 1.0f);
}
