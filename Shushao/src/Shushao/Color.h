#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

namespace se {

class Color {
public:
    Color();
    Color(std::string);
    Color(float r, float g, float b, float a = 1.0f);
    Color(int r, int g, int b, float a = 1.0f);

    float r;  //	Red component of the color.
    float g;  //	Green component of the color.
    float b;  //	Blue component of the color.
    float a;  //	Alpha component of the color.

    void Set(float r, float g, float b, float a = 1.0f);
    void Set(int r, int g, int b, float a = 1.0f);

    Color grayscale();  //	The grayscale value of the color. (Read Only)

    float maxColorComponent();  //	Returns the maximum color component value: Max(r,g,b).
    std::string ToString();

    glm::vec4 rgba();
    glm::vec3 rgb();
    void Clamp();

    bool operator==(Color& other);
    void operator=(const Color& other);
    Color operator*(const Color& other);
    Color operator*(const float value);
    Color operator/(const Color& other);
    Color operator/(const float value);
    Color operator+(const Color& other);
    Color operator+(const float value);
    Color& operator+=(Color const& other) &;

    Color operator-(const Color& other);
    Color operator-(const float value);
    Color& operator-=(Color const& other) &;

    static glm::vec3 hex2rgb(std::string hexcode);
};

namespace color {
    const Color black = {0.0f, 0.0f, 0.0f, 1.0f};
    const Color blue = {0.0f, 0.0f, 1.0f, 1.0f};
    const Color clear = {0.0f, 0.0f, 0.0f, 0.0f};
    const Color cyan = {0.0f, 1.0f, 1.0f, 1.0f};
    const Color gray = {0.5f, 0.5f, 0.5f, 1.0f};
    const Color green = {0.0f, 1.0f, 0.0f, 1.0f};
    const Color grey = {0.5f, 0.5f, 0.5f, 1.0f};
    const Color magenta = {1.0f, 0.0f, 1.0f, 1.0f};
    const Color red = {1.0f, 0.0f, 0.0f, 1.0f};
    const Color white = {1.0f, 1.0f, 1.0f, 1.0f};
    const Color yellow = {1.0f, 0.92f, 0.016f, 1.0f};
}  // namespace color

}  // namespace se
