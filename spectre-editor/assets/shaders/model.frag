#version 450 core

layout (location = 0) out vec4 color;

in vec3 v_FragmentPosition;
in vec2 v_TextureCoordinate;
in vec3 v_Normal;

uniform vec3 u_LightPosition;
uniform vec3 u_MaterialDiffuse;

void main()
{
    vec3 objectColor = u_MaterialDiffuse;
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_LightPosition - v_FragmentPosition);
    float diffuseImpact = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diffuseImpact * lightColor;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(u_LightPosition - v_FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);  
    float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32.0f);
    vec3 specular = specularStrength * specularImpact * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    color = vec4(result, 1.0f);
}
