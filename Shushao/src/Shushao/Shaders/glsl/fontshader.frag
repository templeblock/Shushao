#version 430

in vec2 UV;

layout(location=6) uniform vec4 render_color;

uniform sampler2D diffuse_map;

out vec4 frag_color;

void main(void) {
    frag_color = vec4(1, 1, 1, texture(diffuse_map, UV).r) * render_color;
}
