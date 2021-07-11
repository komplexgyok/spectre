#version 450 core

layout (location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragmentPosition;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPosition;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
    // Ambient
    vec3 ambient = u_Light.ambient * u_Material.ambient;
  	
    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragmentPosition);
    float diffuseImpact = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = u_Light.diffuse * (diffuseImpact * u_Material.diffuse);

    // Specular
    vec3 viewDirection = normalize(u_ViewPosition - v_FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);  
    float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0f), u_Material.shininess);
    vec3 specular = u_Light.specular * (specularImpact * u_Material.specular);  
    
    vec3 result = ambient + diffuse + specular;
    
    color = vec4(result, 1.0f);
}
