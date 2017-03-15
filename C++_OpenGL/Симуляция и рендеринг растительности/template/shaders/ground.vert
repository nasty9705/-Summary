#version 330

in vec4 point;
out vec2 position;

uniform mat4 camera;

void main() {
    gl_Position = camera * point;
position.x = point.x;
position.y = point.z;
}
