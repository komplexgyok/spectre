#version 450 core

layout (location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragmentPosition;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ObjectColor;
uniform vec3 u_ViewPosition;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * u_LightColor;
  	
    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_LightPosition - v_FragmentPosition);
    float diffuseImpact = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diffuseImpact * u_LightColor;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(u_ViewPosition - v_FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);  
    float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32.0f);
    vec3 specular = specularStrength * specularImpact * u_LightColor;  
    
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    
    color = vec4(result, 1.0f);
}
