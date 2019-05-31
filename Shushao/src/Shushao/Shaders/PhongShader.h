#pragma once

#include "Shushao/Core.h"
#include "Shader.h"

namespace se {

class PhongShader : public Shader {
public:
    PhongShader();

protected:
    void Awake();
    void Update();
};

}  // namespace se
