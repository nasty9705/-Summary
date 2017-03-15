#version 330

in vec2 position;
out vec4 outColor;
uniform sampler2D texturetmp;


void main() {
    //outColor = vec4(0.5, 0.5, 0, 0);
outColor = texture(texturetmp, position);
}
