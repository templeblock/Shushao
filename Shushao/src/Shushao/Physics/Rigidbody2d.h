#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Include/spimpl/spimpl.h"
#include "Shushao/Component.h"

namespace se {

enum class RigidbodyType {
    STATIC = 0,
    KINEMATIC = 1,
    DYNAMIC = 2
};

class Rigidbody2D : public Component {
    PLIMPL_FORWARD

public:

    Rigidbody2D();

    virtual void setup() {
        name = "Rigidbody 2D";
    }

    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 velocity = glm::vec2(0, 0);
    float angle = 0.0f;  // DEGREES
    float angularVelocity = 0.0f;

    bool fixedRotation = false;

    void SetType(RigidbodyType type_);
    void SetFixedRotation(bool fr);
    void Copy(Rigidbody2D* other);

    void* GetBody();
    void* GetBodyDef();

    void Awake();
    void Update();
    void FixedUpdate();
    void OnDestroy();
    void OnDisable();
    void OnEnable();

private:
    RigidbodyType type = RigidbodyType::DYNAMIC;
};

}  // namespace se
