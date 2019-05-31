#version 430

// TODO: Spotlight function

#define MAX_POINT_LIGHTS 6
#define MAX_SPOT_LIGHTS 6

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec2 UV;
in vec3 FragPos;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;

uniform bool enabled_diffuse_map;
uniform bool enabled_normal_map;
uniform bool enabled_specular_map;

uniform vec4 ambient_color;
uniform vec4 diffuse_color;
uniform vec4 specular_color;
uniform float shininess;

uniform vec3 view_position;
uniform Light directional_light;
uniform Light point_lights[MAX_POINT_LIGHTS];
uniform Light spot_lights[MAX_POINT_LIGHTS];
uniform int point_lights_number;
uniform int spot_lights_number;

vec4 diffuse_map_fragcolor;
vec4 specular_map_fragcolor;

out vec4 frag_color;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

    // properties
    vec3 norm;
    if (enabled_normal_map) {
        norm = normalize(texture(normal_map, UV).rgb * 2.0 - 1.0);
    } else {
        norm = normalize(Normal);
    }

    vec3 viewDir = normalize(view_position - FragPos);

    // Directional lighting
    vec3 result = CalcDirLight(directional_light, norm, viewDir);

    // Point lights
    for(int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (i >= point_lights_number) break;
        result += CalcPointLight(point_lights[i], norm, FragPos, viewDir);
    }

    // phase 3: Spot light
    /* for(int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        if (i >= spot_lights_number) break;
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    } */

    frag_color = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient, diffuse, specular;

    // combine results
    if (enabled_diffuse_map) {
        ambient = light.ambient * vec3(texture(diffuse_map, UV));
        diffuse = light.diffuse * diff * vec3(texture(diffuse_map, UV));
    } else {
        ambient = light.ambient * vec3(ambient_color);
        diffuse = light.diffuse * diff * vec3(diffuse_color);
    }
    if (enabled_specular_map) {
        specular = light.specular * spec * vec3(texture(specular_map, UV));
    } else {
        specular = light.specular * spec * vec3(specular_color);
    }

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient, diffuse, specular;

    // combine results
    if (enabled_diffuse_map) {
        ambient = light.ambient * vec3(texture(diffuse_map, UV));
        diffuse = light.diffuse * diff * vec3(texture(diffuse_map, UV));
    } else {
        ambient = light.ambient * vec3(ambient_color);
        diffuse = light.diffuse * diff * vec3(diffuse_color);
    }
    if (enabled_specular_map) {
        specular = light.specular * spec * vec3(texture(specular_map, UV));
    } else {
        specular = light.specular * spec * vec3(specular_color);
    }

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}