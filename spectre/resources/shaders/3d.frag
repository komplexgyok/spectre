#version 330 core

out vec4 color;

in vec2 vTextureCoordinate;
in float vTextureId;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;

void main()
{
    if (abs(vTextureId - 1.0f) < 0.00001f) {
        color = texture(uTexture1, vTextureCoordinate);
    }
    else if (abs(vTextureId - 2.0f) < 0.00001f) {
        color = texture(uTexture2, vTextureCoordinate);
    }
    else {
        color = texture(uTexture3, vTextureCoordinate);
    }
    //color = texture(uTexture2, vTextureCoordinate);
}
