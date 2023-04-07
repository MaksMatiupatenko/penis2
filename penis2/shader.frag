#version 400

in vec4 othercolor;
in vec2 texcoord;

uniform sampler2D tex;

void main() {
	gl_FragColor = texture(tex, texcoord);
	//gl_FragColor = vec4(1, 0, 0, 1);
}
