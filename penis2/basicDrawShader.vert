#version 430

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 incolor;
layout (location = 2) in vec4 intexcoord;

out vec4 color;
out vec2 texcoord;

uniform mat3 model;
uniform mat3 view;

void main() {
	gl_Position = vec4((view * model * vec3(pos.xy, 1)).xy, 0.0, 1.0);
	color = incolor;
	texcoord = intexcoord.xy;
}
