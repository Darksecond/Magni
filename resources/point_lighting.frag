#version 150

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    vec3 position; //world space
    Attenuation att;
};

uniform PointLight light;
uniform sampler2D g_diffuse;
uniform sampler2D g_position; //eye space
uniform sampler2D g_normal; //eye space

uniform mat4 view;

uniform vec2 screen_size;

out vec4 final;

vec2 calc_tex_coord()
{
    return gl_FragCoord.xy / screen_size;
}

vec4 calc_point_light(vec3 eye_position, vec3 light_eye_pos, vec3 eye_normal)
{
    vec4 Ia = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 Id = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 Is = vec4(0.0, 0.0, 0.0, 1.0);
    
    vec3 light_direction = eye_position - light_eye_pos;
    float distance = length(light_direction);
    light_direction = normalize(light_direction);
    
    float diffuse_factor = dot(eye_normal, -light_direction);
    if(diffuse_factor > 0)
    {
        Id = vec4(1.0, 1.0, 1.0, 1.0) * diffuse_factor;
        
        vec3 vertex_to_eye = normalize(-eye_position);
        vec3 light_reflect = normalize(reflect(light_direction, eye_normal));
        float specular_factor = dot(vertex_to_eye, light_reflect);
        if(specular_factor > 0)
        {
            Is = vec4(1.0, 1.0, 1.0, 1.0) * specular_factor;
        }
    }
    
    float att = light.att.constant +
                light.att.linear * distance +
                light.att.exp * distance * distance;
    att = min(1.0, att);
    
    
    return (Ia + Id + Is) / att;
    
}

void main()
{
    
    vec3 light_eye_pos = (view * vec4(light.position, 1.0)).xyz;
    
    vec2 tex_coord = calc_tex_coord();
    
    vec3 diffuse = texture(g_diffuse, tex_coord).xyz;
    vec3 eye_position = texture(g_position, tex_coord).xyz;
    vec3 normal = texture(g_normal, tex_coord).xyz;
    
    vec4 color = calc_point_light(eye_position, light_eye_pos, normal);
    
    final = vec4(diffuse, 1.0) * color;
}