#version 330 core

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_txy;
uniform float transparent_index = 0;
uniform int water_shift_1;
uniform int water_shift_2;

in vec2 tex_coords;

out vec4 frag_color;

int add_wrapped(int value, int addend, int range_min, int range_size)
{
    return (value + addend - range_min) % range_size + range_min;
}

int apply_water_shift(int palette_index) {
    // Precondition: 224 <= palette_index < 239
    if (palette_index < 228)
    {
        // Water
        return add_wrapped(palette_index, water_shift_1, 224, 4);
    }
    else if (palette_index < 232)
    {
        // Wakes
        // Use the inverse of water_shift_1
        int shift = 3 - water_shift_1;
        return add_wrapped(palette_index, shift, 228, 4);
    }
    // Coastlines
    return add_wrapped(palette_index, water_shift_2, 232, 7);
}

void main() {
    // palette_index is a lookup into the palette texture (0-1)
    float palette_index = texture(tex, tex_coords).r;
    if (palette_index == transparent_index)
    {
        discard;
    }

    // Get the size of 1 pixel
    int tex_width = textureSize(palette, 0).x;
    float px_size = 1.f / float(tex_width);
    
    // Apply a palette shift for water pixels
    int palette_index_int = int(palette_index * 256);
    if (224 <= palette_index_int && palette_index_int < 239)
    {
        palette_index_int = apply_water_shift(palette_index_int);
        palette_index = palette_index_int * px_size;
    }

    vec2 palette_lookup = vec2(palette_index, palette_txy);
    frag_color = texture(palette, palette_lookup);
}
