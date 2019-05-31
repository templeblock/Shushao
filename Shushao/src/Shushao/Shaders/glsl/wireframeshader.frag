#version 430

in vec4 rendered_color;
out vec4 frag_color;

void main() {
    frag_color = rendered_color;
}
