#version 430

layout(location=6) uniform vec4 render_color;
out vec4 frag_color;

void main() {
    frag_color = render_color;
}