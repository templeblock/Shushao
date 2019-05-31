#pragma once

#include "Core.h"
#include "sepch.h"
#include "Color.h"
#include "Component.h"

namespace se {

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct UniformLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutoff;
};

class Light : public Component {
public:
    enum class Type {
        SPOT,
        DIRECTIONAL,
        POINT,
        RECTANGLE,
        DISC
    };

    virtual void setup() { name = "Light"; }

    bool enabled;
    Light::Type type = Light::Type::POINT;
    glm::vec3 direction;
    Color ambient = color::white;
    Color diffuse = color::white;
    Color specular = color::white;
    Attenuation attenuation{1.0f, 1.0f, 1.0f};
    float cutoff;

    std::string GetTypeDesc();
    UniformLight GetUniform();

    void Copy(Light* other);

protected:
    void Update();
    void Render();
};

}  // namespace se
