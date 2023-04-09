#version 400

in vec4 othercolor;
in vec2 texcoord;

uniform sampler2D tex;
uniform sampler2D tex2;

void main() {
	gl_FragColor = mix(texture(tex, texcoord), texture(tex2, texcoord), 0);
	//gl_FragColor = vec4(1, 0, 0, 1);
}
