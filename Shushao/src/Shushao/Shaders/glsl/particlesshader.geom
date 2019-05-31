#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 color[];
in vec2 size[];

out vec2 UV;
out vec4 out_color;
out vec4 out_size;

uniform mat4 P;

void generateVertex(vec3 v);
void generateQuad();

void main() {
    generateQuad();
}

/* functions */

void generateVertex(vec3 v) {
    gl_Position = gl_in[0].gl_Position + (P * vec4(v, 0.0) * vec4(size[0], 0, 0));
    UV = vec2(v.x - 0.5f, - (v.y + 0.5f));
    out_color = color[0];
    EmitVertex();
}

void generateQuad() {
    vec2 h = {0.5f, 0.5f};
    generateVertex(vec3(-h.x, -h.y, 0.0));
    generateVertex(vec3( h.x, -h.y, 0.0));
    generateVertex(vec3(-h.x,  h.y, 0.0));
    generateVertex(vec3( h.x,  h.y, 0.0));
    EndPrimitive();
}
