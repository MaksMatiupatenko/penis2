#version 430

in vec4 color;
in vec2 texcoord;

uniform sampler2D tex;
uniform int hasTexture = 0;

void main() {
	if (hasTexture == 1) {
		gl_FragColor = texture(tex, texcoord) * color;
	}
	else {
		gl_FragColor = color;
	}
}
