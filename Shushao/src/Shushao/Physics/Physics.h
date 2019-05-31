#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Include/spimpl/spimpl.h"

namespace se {

class Physics {
public:

    PLIMPL_FORWARD_STATIC

    static bool enabled;
    static bool debug;

    static bool doSleep;
    static float timeStep;
    static int velocityIterations;
    static int positionIterations;

    void SetGravity(glm::vec3);

    static bool Init();
    static void Update();
    static void Exit();

    static void* GetWorld();
    static void* GetGravity();
};

}  // namespace se
