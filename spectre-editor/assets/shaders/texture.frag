#version 330 core

out vec4 color;

in vec4 vColor;
in vec2 vTextureCoordinate;

uniform sampler2D sTexture;

void main()
{
    color = texture(sTexture, vTextureCoordinate) * vColor;
}
