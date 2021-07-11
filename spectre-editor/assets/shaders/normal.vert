#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = uView * u_Model * vec4(a_Position, 1.0f); 
    mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * a_Normal, 0.0f)));
}
