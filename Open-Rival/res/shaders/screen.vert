#version 330 core

layout(location = 0) in vec2 in_vertex;
layout(location = 1) in vec2 in_tex_coords;

out vec2 tex_coords;

void main() {
    gl_Position = vec4(in_vertex.x, in_vertex.y, 0, 1);
    tex_coords = in_tex_coords;
}
