#version 400

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 intexcoord;

out vec4 othercolor;
out vec2 texcoord;

uniform mat3 model;
uniform mat3 view;

void main() {
	//gl_Position = vec4((model * view * vec3(pos.xy, 1)).xy, 0.0, 1.0);
	gl_Position = vec4((view * model * vec3(pos.xy, 1)).xy, 0.0, 1.0);
	othercolor = color;
	texcoord = intexcoord.xy;
}
