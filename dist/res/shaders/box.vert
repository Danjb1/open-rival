#version 330 core

uniform mat4 view_proj_matrix;

layout(location = 0) in vec2 in_vertex;
layout(location = 2) in vec4 in_color;

out vec4 color;

void main() {
    gl_Position = view_proj_matrix * vec4(in_vertex.x, in_vertex.y, 0, 1);
    color = in_color;
}
