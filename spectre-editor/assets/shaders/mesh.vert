#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec3 v_FragmentPosition;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_FragmentPosition = vec3(u_Model * vec4(a_Position, 1.0f));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    
    gl_Position = u_Projection * u_View * vec4(v_FragmentPosition, 1.0f);
}
