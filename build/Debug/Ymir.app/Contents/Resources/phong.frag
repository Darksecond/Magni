#version 150

struct Light
{
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

struct Material
{
    vec3  diffuse;
    vec3  ambient;
    vec3  specular;
    vec3  emissive;
    float shininess;
};

uniform Material material;
uniform Light light;

in vec2 fragTexCoord;
in vec3 tnorm;
in vec4 eye;

out vec4 finalColor;

void main() {
    vec3 n     = normalize(tnorm);
    vec3 v     = normalize(-(normalize(eye).xyz));
    float attenuation = 1.0;
    vec3 s = vec3(0.0);
    
    if(0.0 == light.position.w) //directional light
    {
        attenuation = 1.0;
        s = normalize(vec3(light.position));
    }
    else //point light
    {
        vec3 lightToEye = vec3(light.position - eye);
        s = normalize(lightToEye);
        float distance = length(lightToEye);
        attenuation  = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);
    }
    
    vec3 r     = reflect( -s, n);
    
    vec3 ambient = attenuation * light.ambient * material.ambient;
    
    float sDotN = max( dot(s, n), 0.0);
    vec3 diffuse = attenuation * light.diffuse * material.diffuse * sDotN;
    
    vec3 spec   = vec3(0.0);
    if( sDotN > 0.0)
    {
        spec = attenuation * light.specular * material.specular * pow( max(dot(r, v), 0.0), material.shininess);
    }
    vec3 lightIntensity = material.emissive + ambient + diffuse + spec;
    
    finalColor = vec4(lightIntensity, 1.0);

}