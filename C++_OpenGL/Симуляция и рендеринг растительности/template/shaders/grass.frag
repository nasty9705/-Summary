#version 330

in vec2 tmp;
in vec4 send;
out vec4 outColor;
uniform sampler2D tmptexture;

void main() {
    //outColor = vec4(0, 1, 0, 0);
	outColor = texture(tmptexture, tmp)*vec4(send.y + 0.4, send.y + 0.4, send.y + 0.4, 1);
}