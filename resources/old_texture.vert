#version 150

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;

    // Apply all matrix transformations to vert
    gl_Position = projection * view * model * vec4(vert, 1.0);
}