#version 430

layout(location=1) in vec3 vertex_coord;
layout(location=5) uniform mat4 MVP;

uniform mat4 M;

uniform vec2 viewport;
uniform bool enabled_viewport;

void main() {
    if (enabled_viewport) {
        gl_Position = vec4(2 * (vertex_coord.xy / viewport.xy) - 1, 0, 1);

    } else {
        gl_Position = MVP * vec4(vertex_coord, 1.0);
    }
}