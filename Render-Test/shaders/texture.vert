#version 140

in vec2 pos;

void main() {
    gl_Position = vec4(pos.x, pos.y, 0, 1);
}
