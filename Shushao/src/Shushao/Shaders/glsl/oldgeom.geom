#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 color[];
in vec2 size[];

out vec2 UV;
out vec4 out_color;
out vec4 out_size;

uniform mat4 MVP;

//vec2 size = vec2(1.0f, 1.0f);

void generateVertex(vec4 position, float x, float y);
void generateQuad(vec4 position, vec2 size);

void main() {
    generateQuad(gl_in[0].gl_Position, vec2(1.0f, 1.0f));
}

/* functions */

void generateVertex(vec4 position, float x, float y) {
    gl_Position = position + (MVP * vec4(x, y, 0, 0) * vec4(size[0], 0, 0));
    UV = vec2(x - 0.5f, - (y + 0.5f));
    out_color = color[0];
    EmitVertex();
}

void generateQuad(vec4 position, vec2 size) {
    vec2 h = size / 2;
    generateVertex(position, -h.x, -h.y);
    generateVertex(position,  h.x, -h.y);
    generateVertex(position, -h.x,  h.y);
    generateVertex(position,  h.x,  h.y);
    EndPrimitive();
}