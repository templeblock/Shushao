#version 430

in vec2 UV;
in vec4 out_color;

layout(location=6) uniform vec4 render_color;

uniform sampler2D diffuse_map;

out vec4 frag_color;

void main() {
    frag_color = texture(diffuse_map, UV) * out_color * vec4(render_color);
}
