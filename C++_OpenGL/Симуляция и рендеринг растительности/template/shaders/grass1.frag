#version 330

in vec2 tmp1;
in vec4 send1;
out vec4 outColor;
uniform sampler2D tmptexture1;

void main() {
    //outColor = vec4(1, 0, 0, 0);
	outColor = texture(tmptexture1, tmp1)*vec4(send1.y + 0.4, send1.y + 0.4, send1.y + 0.4, 1);
}