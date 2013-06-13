#version 150

uniform sampler2D g_diffuse;
//uniform sampler2D g_position;
//uniform sampler2D g_normal;

uniform vec2 screen_size;

out vec4 final;

vec2 calc_tex_coord()
{
    return gl_FragCoord.xy / screen_size;
}

void main()
{
    vec2 tex_coord = calc_tex_coord();
    
    //vec3 eye_position = texture(g_position, tex_coord).xyz;
    vec3 diffuse = texture(g_diffuse, tex_coord).xyz;
    //vec3 normal = texture(g_normal, tex_coord).xyz;
    
    final = vec4(diffuse, 1.0);
}