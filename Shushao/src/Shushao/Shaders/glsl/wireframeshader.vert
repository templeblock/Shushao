#version 430

layout(location=1) in vec3 vertex_coord;
layout(location=3) in vec4 vertex_color;
uniform layout(location=5) mat4 MVP;

uniform mat4 M;
uniform vec2 viewport;
uniform bool enabled_viewport;

out vec4 rendered_color;

void main() {

    if (enabled_viewport) {
        vec4 translated = M * vec4(vertex_coord.xy, 0.0f, 1.0f);
        gl_Position = vec4(2 * (translated.xy / viewport.xy) - 1, 0, 1);
    } else {
        gl_Position = MVP * vec4(vertex_coord, 1.0);
    }
    rendered_color = vertex_color;
}
