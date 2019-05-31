#version 430

layout(location=1) in vec4 vertex_coord;
layout(location=5) uniform mat4 MVP;

uniform mat4 M;

uniform vec2 viewport;
uniform bool enabled_viewport;

out vec2 UV;

void main(void) {

    if (enabled_viewport) {
        vec4 translated = M * vec4(vertex_coord.xy, 0.0f, 1.0f);
        gl_Position = vec4(2 * (translated.xy / viewport.xy) - 1, 0, 1);
    } else {
        gl_Position = MVP * vec4(vertex_coord.xy, 0.0f, 1.0f);
    }

    UV = vertex_coord.zw;
}
