#version 330

in vec2 tmp2;
in vec4 send2;
out vec4 outColor;
uniform sampler2D tmptexture2;

void main() {
    //outColor = vec4(1, 0, 0, 0);
	outColor = texture(tmptexture2, tmp2)*vec4(send2.y + 0.4, send2.y + 0.4, send2.y + 0.4, 1);
}