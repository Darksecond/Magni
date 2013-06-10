#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;

void main() {
    fragTexCoord = vertTexCoord;
    fragNormal = (model * vec4(vertNormal, 0.0)).xyz;
    fragPos = (model * vec4(vert, 1.0)).xyz;

    gl_Position = projection * view * model * vec4(vert, 1.0);
}