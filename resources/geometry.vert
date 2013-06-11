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
    //normal should be an uniform!
    mat3 normal = transpose(inverse(mat3(view * model)));
    fragNormal = normal * vertNormal;
    fragPos = (view * model * vec4(vert, 1.0)).xyz;

    gl_Position = projection * view * model * vec4(vert, 1.0);
}