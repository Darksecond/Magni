#version 150

uniform sampler2D diffuse;

in vec2 fragTexCoord;

out vec4 finalColor;

void main()
{
    finalColor = texture(diffuse, fragTexCoord);
}