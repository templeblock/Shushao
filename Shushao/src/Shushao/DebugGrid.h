#pragma once

#include "Entity.h"

namespace se {

class DebugGrid : public Entity {
public:
    void Awake();

    float gridSize = 1000.0f;
    float gizmoMagnitude = 2.0f;
};

}  // namespace se
