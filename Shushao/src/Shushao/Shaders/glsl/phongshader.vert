#version 430

layout(location=1) in vec3 vertex_coord;
layout(location=2) in vec2 texture_coord;
layout(location=4) in vec3 normal_value;

layout(location=5) uniform mat4 MVP;
uniform mat4 M;

out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = MVP * vec4(vertex_coord, 1.0);
    FragPos = vec3(M * vec4(vertex_coord, 1.0));
    UV = texture_coord;
    Normal = mat3(transpose(inverse(M))) * normal_value;
    // Normal = normal_value;

    // FIXME: verificare che l'uso di M sopra ora funzioni
}