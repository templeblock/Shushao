#version 430

in vec2 UV;

layout(location=6) uniform vec4 render_color;

uniform sampler2D diffuse_map;

// slice/tile options
uniform bool enabled_sliced;
uniform bool enabled_tiled;
uniform vec4 sprite_border; // X=left, Y=bottom, Z=right, W=top
uniform vec4 image_border; // X=left, Y=bottom, Z=right, W=top
uniform vec2 texture_size;
uniform vec2 image_size;
uniform bool sliced_fill;

out vec4 frag_color;

float map(float value, float originalMin, float originalMax, float newMin, float newMax);
vec4 scaleBorders(vec4 pixelBorder, vec2 pixelSize);
vec2 processSliced(vec2 uv);
vec2 processTiled(vec2 uv);

void main() {

    if (enabled_sliced) {

        vec2 newUV = processSliced(UV);
        frag_color = texture(diffuse_map, newUV) * vec4(render_color);
        return;

    } else if (enabled_tiled) {

        vec2 newUV = processTiled(UV);
        frag_color = texture(diffuse_map, newUV) * vec4(render_color);
        return;
    }

    frag_color = texture(diffuse_map, UV) * vec4(render_color);
}

/*
    functions
*/

float map(float value, float originalMin, float originalMax, float newMin, float newMax) {
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

vec4 scaleBorders(vec4 border, vec2 size) {
    return vec4(
        border.x / size.x,
        border.y / size.y,
        border.z / size.x,
        border.w / size.y
    );
}

vec2 processSliced(vec2 uv) {

    // sprite borders in uv coords
    vec4 uvSpriteBorder = scaleBorders(sprite_border, texture_size);

    // image borders in uv coords
    vec4 uvImageBorder = scaleBorders(image_border, image_size);

    bool middleX = false, middleY = false;

    if (uv.x <= uvImageBorder.x) {
        uv.x = map(uv.x, 0, uvImageBorder.x, 0, uvSpriteBorder.x);
    } else if (uv.x < 1 - uvImageBorder.z) {
        uv.x = map(uv.x, uvImageBorder.z, 1 - uvImageBorder.z, uvSpriteBorder.z, 1 - uvSpriteBorder.z);
        middleX = true;
    } else {
        uv.x = map(uv.x, 1 - uvImageBorder.z, 1, 1 - uvSpriteBorder.z, 1);
    }

    if (uv.y <= uvImageBorder.w) {
        uv.y = map(uv.y, 0, uvImageBorder.w, 0, uvSpriteBorder.w);
    } else if (uv.y < 1 - uvImageBorder.y) {
        uv.y = map(uv.y, uvImageBorder.y, 1 - uvImageBorder.y, uvSpriteBorder.y, 1 - uvSpriteBorder.y);
        middleY = true;
    } else {
        uv.y = map(uv.y, 1 - uvImageBorder.y, 1, 1 - uvSpriteBorder.y, 1);
    }

    if (!sliced_fill && middleX && middleY) {
        discard;
    }

    return uv;
}

vec2 processTiled(vec2 uv) {

    // sprite borders in uv coords
    vec4 uvSpriteBorder = scaleBorders(sprite_border, texture_size);

    // image borders in uv coords
    vec4 uvImageBorder = scaleBorders(image_border, image_size);

    // texture scaling calcolato sui bordi della image
    vec4 textureScaling = image_border / sprite_border;

    // pixel coord on the Image
    vec2 imageCoord = image_size * uv;

    // size delle side (esclusi angoli) scalate sulle image size
    float textureSide = (texture_size.y - sprite_border.w - sprite_border.y) * textureScaling.y;
    float textureFloor = (texture_size.x - sprite_border.x - sprite_border.z) * textureScaling.x;

    bool middleX = false, middleY = false;

    // TOPLEFT CORNER
    if (uv.x <= uvImageBorder.x && uv.y <= uvImageBorder.w) {
        uv.x = map(uv.x, 0, uvImageBorder.x, 0, uvSpriteBorder.x);
        uv.y = map(uv.y, 0, uvImageBorder.w, 0, uvSpriteBorder.w);

    // TOPRIGHT CORNER
    } else if (uv.x >= 1 - uvImageBorder.z && uv.y <= uvImageBorder.w) {
        uv.x = map(uv.x, 1 - uvImageBorder.z, 1, 1 - uvSpriteBorder.z, 1);
        uv.y = map(uv.y, 0, uvImageBorder.w, 0, uvSpriteBorder.w);

    // BOTTOMLEFT CORNER
    } else if (uv.x <= uvImageBorder.x && uv.y >= 1 - uvImageBorder.y) {
        uv.x = map(uv.x, 0, uvImageBorder.x, 0, uvSpriteBorder.x);
        uv.y = map(uv.y, 1 - uvImageBorder.y, 1, 1 - uvSpriteBorder.y, 1);

    // BOTTOMRIGHT CORNER
    } else if (uv.x >= 1 - uvImageBorder.z && uv.y >= 1 - uvImageBorder.y) {
        uv.x = map(uv.x, 1 - uvImageBorder.z, 1, 1 - uvSpriteBorder.z, 1);
        uv.y = map(uv.y, 1 - uvImageBorder.y, 1, 1 - uvSpriteBorder.y, 1);

    // MIDDLE AREA
    } else  if (uv.x > uvImageBorder.x && uv.y < 1 - uvImageBorder.y && uv.x < 1 - uvImageBorder.z && uv.y > uvImageBorder.w) {
        middleX = middleY = true;
        uv.x = (int(imageCoord.x) % int(textureFloor) + image_border.x) / (texture_size.x * textureScaling.x);
        uv.y = (int(imageCoord.y) % int(textureSide) + image_border.w) / (texture_size.y * textureScaling.y);

    // LEFT SIDE
    } else if (uv.x <= uvImageBorder.x && uv.y < 1 - uvImageBorder.y && uv.y > uvImageBorder.w) {

        uv.x = map(uv.x, 0, uvImageBorder.x, 0, uvSpriteBorder.x);
        uv.y = (int(imageCoord.y) % int(textureSide) + image_border.w) / (texture_size.y * textureScaling.y);

    // RIGHT SIDE
    } else if (uv.x >= 1 - uvImageBorder.z && uv.y < 1 - uvImageBorder.y && uv.y > uvImageBorder.w) {
        uv.x = map(uv.x, 1 - uvImageBorder.z, 1, 1 - uvSpriteBorder.z, 1);
        uv.y = (int(imageCoord.y) % int(textureSide) + image_border.w) / (texture_size.y * textureScaling.y);

    // TOP SIDE
    } else if (uv.x > uvImageBorder.x && uv.x < 1 - uvImageBorder.z && uv.y <= uvImageBorder.w) {
        uv.x = (int(imageCoord.x) % int(textureFloor) + image_border.x) / (texture_size.x * textureScaling.x);
        uv.y = map(uv.y, 0, uvImageBorder.w, 0, uvSpriteBorder.w);

    // BOTTOM SIDE
    } else if (uv.x > uvImageBorder.x && uv.x < 1 - uvImageBorder.z && uv.y >= 1 - uvImageBorder.y) {
        uv.x = (int(imageCoord.x) % int(textureFloor) + image_border.x) / (texture_size.x * textureScaling.x);
        uv.y = map(uv.y, 1 - uvImageBorder.y, 1, 1 - uvSpriteBorder.y, 1);
    }

    if (!sliced_fill && middleX && middleY) {
        discard;
    }

    return uv;
}
