#version 150

uniform mat4 projection;
uniform mat4 modelView;
uniform mat3 normal;

in vec3 vert;
in vec3 vertNormal;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

out vec3 tnorm;
out vec4 eye;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;
    
    tnorm = normalize(normal * vertNormal);
    eye   = modelView * vec4(vert, 1.0);
    
    // Apply all matrix transformations to vert
    gl_Position = projection * modelView * vec4(vert, 1.0);
}