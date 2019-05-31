#pragma once

#include "Camera.h"
#include "Entity.h"

namespace se {

class MainCamera : public Entity {
public:
    void Awake();

    Camera* camera;
};

}  // namespace se
