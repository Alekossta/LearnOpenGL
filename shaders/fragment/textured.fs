#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}

uniform DirLight dirLight;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // light maps
    vec3 diffuseLightMap = vec3(texture(material.diffuse, TexCoords));
    vec3 specularLightMap = vec3(texture(material.specular, TexCoords));

    // final results
    vec3 ambient = light.ambient * diffuseLightMap;
    vec3 diffuse = light.diffuse * diff * diffuseLightMap;
    vec3 specular = light.specular * spec * specularLightMap;

    return ambient + diffuse + specular;
}

void main()
{
    // texture
    vec3 diffuseTexture = vec3(texture(material.diffuse, TexCoords));
    vec3 specularTexture = vec3(texture(material.specular, TexCoords));

    // ambient
    vec3 ambient = diffuseTexture * light.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseTexture * light.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularTexture * spec * light.specular;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}