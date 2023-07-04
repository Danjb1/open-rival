#version 330 core

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_txy;
uniform int transparent_index = 0;
uniform int water_shift_1;
uniform int water_shift_2;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    float palette_index = texture(tex, tex_coords).r;
    if (palette_index == transparent_index || palette_index == 1)
    {
        discard;
    }

    // Get the size of 1 pixel
    ivec2 tex_size_2d = textureSize(palette, 0);
    float px_size = 1.f / float(tex_size_2d.x);

    // Is this a water pixel?
    int palette_index_int = int(palette_index * 256);
    if (palette_index_int >= 224 && palette_index_int < 232)
    {
        palette_index += water_shift_1 * px_size;
    }
    else if (palette_index_int >= 232 && palette_index_int < 239)
    {
        palette_index += water_shift_2 * px_size;
    }
    
    vec2 palette_lookup = vec2(palette_index, palette_txy);
    frag_color = texture(palette, palette_lookup);
}
