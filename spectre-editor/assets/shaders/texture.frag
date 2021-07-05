#version 450 core

out vec4 color;

in vec4 vColor;
in vec2 vTextureCoordinate;
in float vTextureId;

uniform sampler2D uTextures[2];

void main()
{
    int index = int(vTextureId);
    color = texture(uTextures[index], vTextureCoordinate) * vColor;
}
