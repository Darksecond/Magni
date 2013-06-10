#version 150

uniform sampler2D diffuse;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 g_pos;
out vec4 g_diff;
out vec4 g_norm;

void main()
{
    g_pos = vec4(fragPos, 1.0);
    g_diff = texture(diffuse, fragTexCoord);
    g_norm = vec4(normalize(fragNormal), 1.0);
}