#version 150

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightAttenuation;

uniform vec3 materialDiffuse;
uniform vec3 materialAmbient;
uniform vec3 materialSpecular;
uniform vec3 materialEmissive;
uniform float materialShininess;

uniform vec4 light;

in vec2 fragTexCoord;
in vec3 tnorm;
in vec4 eye;

out vec4 finalColor;

void main() {
    vec3 n     = normalize(tnorm);
    vec3 v     = normalize(-(normalize(eye).xyz));
    float attenuation = 1.0;
    vec3 lightToEye = vec3(light - eye);
    vec3 s = normalize(lightToEye);
    vec3 r     = reflect( -s, n);
    float distance = length(lightToEye);
    
    if(0.0 == light.w)
    {
        attenuation = 1.0;
    }
    else
    {
        attenuation  = 1.0 / (lightAttenuation.x + lightAttenuation.y * distance + lightAttenuation.z * distance * distance);
    }
    
    vec3 ambient = attenuation * lightAmbient * materialAmbient;
    
    float sDotN = max( dot(s, n), 0.0);
    vec3 diffuse = attenuation * lightDiffuse * materialDiffuse * sDotN;
    
    vec3 spec   = vec3(0.0);
    if( sDotN > 0.0)
    {
        spec = attenuation * lightSpecular * materialSpecular * pow( max(dot(r, v), 0.0), materialShininess);
    }
    vec3 lightIntensity = materialEmissive + ambient + diffuse + spec;
    
    //note: the texture function was called texture2D in older versions of GLSL
    finalColor = vec4(lightIntensity, 1.0);

}