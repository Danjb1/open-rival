#version 330 core

uniform sampler2D tex;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    frag_color = texture(tex, tex_coords);
}
