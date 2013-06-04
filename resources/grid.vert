#version 150

uniform mat4 MVP;

in vec3 vert;

void main(){

	// Apply matrix transformations to vert
	gl_Position = MVP * vec4(vert, 1.0);

}

