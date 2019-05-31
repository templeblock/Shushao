#version 430

layout(location=1) in vec3 vertex_coord;
layout(location=2) in vec2 texture_coord;

layout(location=5) uniform mat4 MVP;

out vec2 UV;

void main() {
    gl_Position = MVP * vec4(vertex_coord, 1.0);
    UV = texture_coord;
}