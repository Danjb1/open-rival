#version 330 core

uniform mat4 view_proj_matrix;

layout(location = 0) in vec2 in_vertex;
layout(location = 1) in vec2 in_tex_coords;

out vec2 tex_coords;

void main() {
    gl_Position = view_proj_matrix * vec4(in_vertex.x, in_vertex.y, 0, 1);
    tex_coords = in_tex_coords;
}
