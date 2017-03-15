#version 330

in vec2 tmp3;
in vec4 send3;
out vec4 outColor;
uniform sampler2D tmptexture3;

void main() {
    //outColor = vec4(0, 0, 1, 0);
	outColor = texture(tmptexture3, tmp3);
}