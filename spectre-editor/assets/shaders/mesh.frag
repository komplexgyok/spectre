#version 450 core

layout (location = 0) out vec4 color;

in vec3 v_FragmentPosition;
in vec2 v_TextureCoordinate;
in vec3 v_Normal;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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
uniform float u_Time;

void main()
{
    // Ambient
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TextureCoordinate).rgb;
  	
    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragmentPosition);
    float diffuseImpact = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = u_Light.diffuse * diffuseImpact * texture(u_Material.diffuse, v_TextureCoordinate).rgb;

    // Specular
    vec3 viewDirection = normalize(u_ViewPosition - v_FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0f), u_Material.shininess);
    vec3 specular = u_Light.specular * specularImpact * texture(u_Material.specular, v_TextureCoordinate).rgb;
    
    // Emission
    //vec3 emission = texture(u_Material.emission, v_TextureCoordinate + vec2(0.0f, u_Time)).rgb * floor(vec3(1.0f) - texture(u_Material.specular, v_TextureCoordinate).rgb);
    //vec3 emission = vec3(0.0f);
    //if (texture(u_Material.specular, v_TextureCoordinate).r == 0.0f) {
        //emission = texture(u_Material.emission, v_TextureCoordinate).rgb;
        //emission = texture(u_Material.emission, v_TextureCoordinate + vec2(0.0f, u_Time)).rgb;
        //emission = emission * (sin(u_Time) * 0.5f + 0.5f) * 2.0f;
    //}

    vec3 result = ambient + diffuse + specular;// + emission;
    
    color = vec4(result, 1.0f);
}
