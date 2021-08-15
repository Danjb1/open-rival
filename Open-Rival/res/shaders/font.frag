#version 330 core

uniform sampler2D tex;

in vec3 color;
in vec2 tex_coords;

out vec4 frag_color;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, tex_coords).r);
    frag_color = vec4(color, 1.0) * sampled;
}
