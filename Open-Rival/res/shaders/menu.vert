#version 330

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec2 in_tex_coord;

out vec2 tex_coord;

void main() {
    gl_Position = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1);
    tex_coord = in_tex_coord;
}
