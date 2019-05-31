#pragma once

#include "Shushao/Object.h"

namespace se {

class Entity;
class Collider2D;
class Rigidbody2D;
class Transform;

class Collision2D : public Object {
public:
    Collider2D* collider;  //	The incoming Collider2D involved in the collision with the otherCollider.
    glm::vec2 normal;
    std::vector<glm::vec2> contacts;  //	The specific points of contact with the incoming Collider2D. You should avoid using this as it produces memory garbage. Use GetContacts instead.
    bool enabled;  //Indicates whether the collision response or reaction is enabled or disabled.
    Entity* entity;  // The incoming GameObject involved in the collision.
    Collider2D* otherCollider;  // The other Collider2D involved in the collision with the collider.
    Rigidbody2D* otherRigidbody;  // The other Rigidbody2D involved in the collision with the rigidbody.
    glm::vec2 relativeVelocity;  // The relative linear velocity of the two colliding objects (Read Only).
    Rigidbody2D* rigidbody;  // The incoming Rigidbody2D involved in the collision with the otherRigidbody.
    Transform* transform;  // The Transform of the incoming object involved in the collision.
};

}  // namespace se
