#version 150

struct PointLight
{
    vec3 position; //world space
    float radius;
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

vec4 calc_point_light(vec3 v, vec3 light_pos, vec3 N)
{
    N = normalize(N);
    vec4 Ia = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 Id = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 Is = vec4(0.0, 0.0, 0.0, 1.0);
    
    vec3 L = light_pos - v;
    float distance = length(L);
    L = normalize(L);
    vec3 R = normalize(reflect(-L, N));
    vec3 E = normalize(-v);
    
    float diffuse_factor = dot(N, L);
    Id = vec4(1.0, 1.0, 1.0, 1.0) * max(dot(N, L), 0.0); //vec4 is color
    Is = vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,E), 0.0), 0.3 * 60.0); //60.0 == shininess, vec4 is color
    
    Id = clamp(Id, 0.0, 1.0);
    Is = clamp(Is, 0.0, 1.0);
    
    //float att = 1.0 / pow((max(distance - light.radius, 0.0) / light.radius + 1.0), 2.0);
    //float att = (1.0 - pow(pow(distance/light.radius, 2), 3));
    float att = 1.0 - clamp(distance / light.radius, 0.0, 1.0);
    //float att = 1.0;
    //att = clamp(att, 0.0, 1.0);
    
    return(Ia + Id + Is) * att;
    
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