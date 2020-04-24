#version 330

uniform sampler2D tex;
uniform sampler2D palette;

in vec2 tex_coord;

layout(location = 0) out vec4 frag_colour;

void main() {
    vec4 palette_idx = texture(tex, tex_coord);
    frag_colour = texture(palette, vec2(palette_idx.r, 0));
}
