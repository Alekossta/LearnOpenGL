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

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform Material material;
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

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // light maps
    vec3 diffuseLightMap = vec3(texture(material.diffuse, TexCoords));
    vec3 specularLightMap = vec3(texture(material.specular, TexCoords));

    // results 
    vec3 ambient = light.ambient * diffuseLightMap;
    vec3 diffuse = light.diffuse * diff * diffuseLightMap;
    vec3 specular = light.specular * specularLightMap * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    result += CalcPointLight(pointLights[0], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}