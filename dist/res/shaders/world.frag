#version 330 core

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_txy;
uniform int transparent_index = 0;
uniform int water_shift_1;
uniform int water_shift_2;

in vec2 tex_coords;

out vec4 frag_color;

int add_wrapped(int value, int addend, int range_min, int range_size)
{
    return (value + addend - range_min) % range_size + range_min;
}

void main() {
    float palette_index = texture(tex, tex_coords).r;
    if (palette_index == transparent_index || palette_index == 1)
    {
        discard;
    }

    // Get the size of 1 pixel
    int tex_width = textureSize(palette, 0).x;
    float px_size = 1.f / float(tex_width);
    
    // Apply a palette shift for water pixels
    int palette_index_int = int(palette_index * 256);
    if (palette_index_int >= 224 && palette_index_int < 228)
    {
        // Water
        palette_index_int = add_wrapped(palette_index_int, water_shift_1, 224, 4);
        palette_index = palette_index_int * px_size;
    }
    else if (palette_index_int >= 228 && palette_index_int < 232)
    {
        // Wakes
        // Use the inverse of water_shift_1
        int shift = 3 - water_shift_1;
        palette_index_int = add_wrapped(palette_index_int, shift, 228, 4);
        palette_index = palette_index_int * px_size;
    }
    else if (palette_index_int >= 232 && palette_index_int < 239)
    {
        // Coastlines
        palette_index_int = add_wrapped(palette_index_int, water_shift_2, 232, 7);
        palette_index = palette_index_int * px_size;
    }

    vec2 palette_lookup = vec2(palette_index, palette_txy);
    frag_color = texture(palette, palette_lookup);
}
