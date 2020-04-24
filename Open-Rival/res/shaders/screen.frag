#version 330

uniform sampler2D tex;

in vec2 tex_coord;

layout(location = 0) out vec4 frag_colour;

void main() {
    frag_colour = texture(tex, tex_coord);
}
