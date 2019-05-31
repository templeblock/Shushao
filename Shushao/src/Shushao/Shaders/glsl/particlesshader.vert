#version 430

layout(location=1) in vec3 particle_position;
layout(location=3) in vec4 particle_color;
layout(location=7) in vec2 particle_size;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform bool enabled_worldspace;

out vec4 color;
out vec2 size;

void main() {
    gl_Position = (enabled_worldspace ? P*V : P*V*M) * vec4(particle_position, 1.0);

    color = particle_color;
    size = particle_size;
}