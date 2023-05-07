#version 330 core

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_txy;
uniform int transparent_index = 0;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    float palette_index = texture(tex, tex_coords).r;
    if (palette_index == transparent_index || palette_index == 1)
    {
        discard;
    }
    vec2 palette_lookup = vec2(palette_index, palette_txy);
    frag_color = texture(palette, palette_lookup);
}
