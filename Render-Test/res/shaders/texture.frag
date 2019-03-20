#version 330

uniform sampler2D tex;

in vec2 tex_coord;

out vec4 frag_colour;

void main() {
    frag_colour = texture(tex, tex_coord);
}
