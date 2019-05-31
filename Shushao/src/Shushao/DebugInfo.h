#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Text.h"

namespace se {

class DebugInfo : public Component {
public:
    Text* times = nullptr;

    double updateTime;
    double renderTime;
    double frameRate;
    double fixedTime;

protected:
    void Awake();
    void Update();
};

}  // namespace se
