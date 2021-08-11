#version 330 core

uniform sampler2D tex;
uniform vec3 text_colour;

in vec2 tex_coord;

layout(location = 0) out vec4 frag_colour;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    frag_colour = vec4(textColor, 1.0) * sampled;
}
