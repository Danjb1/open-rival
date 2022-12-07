#version 330 core

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_txy;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    vec4 palette_idx = texture(tex, tex_coords);
    if (palette_idx.r == 1) {
        discard;
    }
    frag_color = texture(palette, vec2(palette_idx.r, palette_txy));
}
