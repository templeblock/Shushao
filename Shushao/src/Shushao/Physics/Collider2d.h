#pragma once

#include "Shushao/Component.h"

namespace se {

class Rigidbody2D;

class Collider2D : public Component {
public:
    ~Collider2D();
    virtual void setup();

    Collider2D();

    Rigidbody2D* rigidbody = nullptr;

    glm::vec2 scale = {1.0f, 1.0f};

    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.6f;

    bool isSensor = false;

    // impl
    void* fixtureDef;
    void* fixture = nullptr;
    void* shape;

    void SetDensity(float);
    void SetRestitution(float);
    void SetFriction(float);
    void SetSensor(bool);

    void Awake();
    void Start();

    void Attach();
    void Reset();

    virtual void ResetShape() {}

    void FixedUpdate();
    void OnDestroy();

    void ColliderEnter(Collider2D*);
    void ColliderExit(Collider2D*);

    void Copy(Collider2D* other) {
        if (other == nullptr) return;
        Component::Copy(other);

        scale = other->scale;
        density = other->density;
        friction = other->friction;
        restitution = other->restitution;
        isSensor = other->isSensor;
    }

protected:
    std::vector<Collider2D*> colliders;
};

class PolygonCollider2D : public Collider2D {
public:
    // TODO
};

class BoxCollider2D : public Collider2D {
public:
    BoxCollider2D();

    glm::vec2 boxHalfSize;

    void SetShape(glm::vec2);
    void ResetShape();
    void Copy(BoxCollider2D* other);
};

class CircleCollider2D : public Collider2D {
public:
    CircleCollider2D();
    glm::vec2 position;
    float radius;

    void SetShape(glm::vec2 position_, float radius_);
    void ResetShape();
    void Copy(CircleCollider2D* other);
};

class EdgeCollider2D : public Collider2D {
public:
    EdgeCollider2D();

    void SetShape(glm::vec2, glm::vec2);
    void ResetShape();
    void Copy(EdgeCollider2D* other);
};
}  // namespace se
